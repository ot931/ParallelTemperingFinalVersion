#include "defines.h"

#include <string_view>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <string>
#include <bitset>
#include <gmpxx.h>
#include <chrono>
#include <omp.h>
#include <argumentum/argparse.h>
#include "PartArray.h"
#include "Part.h"
#include "CorrelationCore.h"
#include "CorrelationPointCore.h"
#include "MagnetisationCore.h"
#include "MagnetisationLengthCore.h"
#include "CommandLineParameters.h"
#include "ConfigManager.h"
#include "CalculationParameter.h"
#include <inicpp/inicpp.h>
#include <numeric>
#include "misc.h"
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include "interpolation_class.h"

struct monteCarloStatistics {
        double initEnergy;
        double lowerEnergy;
        double deltaEnergy;
        bool foundLowerEnergy;
        int temperatureOfLowerEnergy;
        string lowerEnergyState;
        vector<string> finalStates;
        vector<double> finalEnergies;
        vector<std::chrono::time_point<std::chrono::steady_clock>> temperature_times_start;
        vector<std::chrono::time_point<std::chrono::steady_clock>> temperature_times_end;
};

std::string xorstr(std::string s1,std::string s2){
        std::string s(s1);
        for (int i=0; i<s1.size(); i++){
                s[i] = (s1[i]==s2[i])?'0':'1';
        }
        return s;
}

std::optional<ConfigManager> readParameters(int argc, char *argv[]){

        // get file name
        bool parse_failed = false;

        auto parser = argumentum::argument_parser{};
        parser.config().program("metropolis").description("Program for calculating heat capacity \
            and magnetisation of spin system with dipole-dipole hamiltonian v." +
                                                                                                          std::string(METROPOLIS_VERSION));
        auto commandLineParameters = std::make_shared<CommandLineParameters>();
        parser.params().add_parameters(commandLineParameters);

        auto parseResult = parser.parse_args(argc, argv, 1);

        if (!parseResult)
        {
                if (commandLineParameters && commandLineParameters->showExample)
                {
                        std::cout << endl;
                        std::cout << "##########################################" << endl;
                        std::cout << "######## contents of example.ini: ########" << endl;
                        std::cout << "##########################################" << endl;
                        std::cout << endl;
                        std::cout << example_string << endl;
                }
                return {};
        }

        inicpp::config iniconfig;
        if (!commandLineParameters->inifilename.empty())
        {
                iniconfig = inicpp::parser::load_file(commandLineParameters->inifilename);
        }

        ConfigManager config = ConfigManager::init(*(commandLineParameters.get()), iniconfig);

        bool configError = config.check_config();
        if (!configError)
        {
                cerr << "Program stopped with error" << endl;
                return {};
        } else {
                config.printHeader();
        }

        return config;
}

monteCarloStatistics montecarlo(ConfigManager &config, bool flag, int swapConst, std::vector<double> &probabilities)
{
        std::mt19937 gen((int)time(0));
        std::uniform_real_distribution<> urd(0., 1.);

        std::ofstream out_c;
        std::ofstream out_data;

        std::ofstream _out_data;
        std::ofstream _out_c;
	//int i_exchange = 0;
/*
        _out_c.open("Capacities.txt");
        _out_c.close();

        _out_data.open("Apamea_output.txt");
        _out_data.close();
*/

        monteCarloStatistics statData;
        unsigned temperatureCount = config.temperatures.size();

        statData.foundLowerEnergy = false;
        statData.finalStates.resize(temperatureCount);
        statData.finalEnergies.resize(temperatureCount);

        statData.temperature_times_start.resize(temperatureCount);
        statData.temperature_times_end.resize(temperatureCount);



        double eOld;


        std::vector<double> e_tt_array (temperatureCount, 0.0);
        std::vector<double> e_tt_array_squared (temperatureCount, 0.0);

        std::vector<double> e_array (temperatureCount, 0.0);
        std::vector<double> t_array = config.temperatures;

        std::vector<std::string> s_array (temperatureCount);

        for (int i = 0; i < temperatureCount; i++) e_array[i] = 0;

        { // block to get initial energy
                const Vect field = config.getField();
                PartArray sys(config.getSystem());
                if (config.isCSV()){
                        ConfigManager::setCSVEnergies(sys);
                } else {
                        if (config.isPBC())
                        {
                                ConfigManager::setPBCEnergies(sys);
                        }
                }
                statData.initEnergy = sys.E();
                for (auto p : sys.parts)
                {
                        statData.initEnergy -= p->m.scalar(field);
                }
                statData.lowerEnergy = statData.initEnergy;
                statData.deltaEnergy = fabs(statData.initEnergy * config.getRestartThreshold());
        }

	std::cout << "debug works. Temperature size: " << config.temperatures.size() << " . Calculation steps: " << config.getCalculate() << std::endl;
	int m_steps = config.getCalculate()/swapConst;

        for (int m_ = 0; m_ < m_steps; m_++)
        {
		std::cout <<"Monte-carlo step: " << m_ + 1 << "/" << m_steps << std::endl;
                #pragma omp parallel
                {
                        #pragma omp for
                        for (int tt = 0; tt < config.temperatures.size(); ++tt)
                        {
                                {
                                        statData.temperature_times_start[tt] = std::chrono::steady_clock::now();

					std::vector<std::unique_ptr<CalculationParameter>> calculationParameters;
                                        config.getParameters(calculationParameters);

                                        const double t = config.temperatures[tt];
                                        const unsigned trseed = config.getSeed() + tt;
                                        default_random_engine generator;
                                        generator.seed(trseed);
                                        uniform_int_distribution<int> intDistr(0, config.N() - 1); // including right edge
                                        uniform_real_distribution<double> doubleDistr(0, 1);       // right edge is not included

       	 				mpf_class e(0, 1024 * 8);
				        mpf_class e2(0, 2048 * 8);

                                        /////////// duplicate the system
                                        PartArray sys(config.getSystem());
                                        if (config.isCSV()){
                                                ConfigManager::setCSVEnergies(sys);
                                        } else {
                                                if (config.isPBC())
                                                {
                                                        ConfigManager::setPBCEnergies(sys);
                                                }
                                        }

                                        // print neighbours and energies
                                        /*sys.E();
                                        for (unsigned i=0; i<sys.size(); i++){
                                                cout<<i<<": ";
                                                unsigned j=0;
                                                for (auto p: sys.neighbours[i]){
                                                        cout<<p->Id()<<"("<<sys.eAt(i,j)<<"), ";
                                                        ++j;
                                                }
                                                cout<<endl;
                                        }*/

                                        const unsigned N = sys.size();

                                        bool swapRes;
                                        unsigned swapNum;
                                        const Vect field = config.getField();

                                        double dE, p, randNum;

                                        double mxOld;
                                        double myOld;

                                        bool acceptSweep;
                                        if (m_ == 0)
                                        {      
                                                // full recalculte energy
                                                eOld = sys.E();
                                                // add external field
                                                for (auto p : sys.parts)
                                                {
                                                        eOld -= p->m.scalar(field);
                                                }                                          
                                                unsigned calculateSteps = config.getHeatup();
                                                for (unsigned step = 0; step < calculateSteps; ++step)
                                                {
                                                        //std::cout << "all works " << std::endl;
                                                        /*
                                                        if (omp_get_thread_num() == 0)
                                                        {
                                                                std::cout << "sys energy: " << sys.E() << std::endl;
                                                        }
                                                        */
                                                        // full recalculte energy every to avoid FP error collection
                                                        if (step != 0 && step % FULL_REFRESH_EVERY == 0)
                                                        {
                                                                eOld = sys.E();
                                                                // add external field
                                                                for (auto p : sys.parts)
                                                                {
                                                                        eOld -= p->m.scalar(field);
                                                                }

                                                                if (statData.foundLowerEnergy){
                                                                        //cancel the calculations
                                                                        break; //break up the main for loop
                                                                }
                                                        }

                                                        for (unsigned sstep = 0; sstep < N; ++sstep)
                                                        {

                                                                dE = 0;
                                                                swapNum = intDistr(generator);
                                                                Part *partA = sys.getById(swapNum);

                                                                { // get dE
                                                                        unsigned j = 0;

                                                                        if (sys.interactionRange() != 0.0)
                                                                        {
                                                                                for (Part *neigh : sys.neighbours[swapNum])
                                                                                {
                                                                                        if (neigh->state == partA->state) // assume it is rotated, inverse state in mind
                                                                                                dE -= 2. * sys.eAt(swapNum, j);
                                                                                        else
                                                                                                dE += 2. * sys.eAt(swapNum, j);
                                                                                        ++j;
                                                                                }
                                                                        }
                                                                        else
                                                                        {
                                                                                for (Part *neigh : sys.parts)
                                                                                {
                                                                                        if (partA != neigh)
                                                                                        {
                                                                                                if (neigh->state == partA->state)
                                                                                                        dE -= 2. * sys.eAt(swapNum, j);
                                                                                                else
                                                                                                        dE += 2. * sys.eAt(swapNum, j);
                                                                                                ++j;
                                                                                        }
                                                                                }
                                                                        }

                                                                        dE += 2 * partA->m.scalar(field);
                                                                }

                                                                acceptSweep = false;
                                                                if (dE < 0 || t == 0)
                                                                {
                                                                        acceptSweep = true;
                                                                }
                                                                else
                                                                {
                                                                        p = exp(-dE / t);
                                                                        randNum = doubleDistr(generator);
                                                                        if (randNum <= p)
                                                                        {
                                                                                acceptSweep = true;
                                                                        }
                                                                }

                                                                if (acceptSweep)
                                                                {
                                                                        sys.parts[swapNum]->rotate(false);
                                                                        eOld += dE;

                                                                        if (config.debug)
                                                                        {
                                                                                // recalc energy
                                                                                double eTmp = sys.E();

                                                                                // add external field
                                                                                for (auto pt : sys.parts)
                                                                                {
                                                                                        eTmp -= pt->m.scalar(field);
                                                                                }
                                                                                /*
                                                                                if (fabs(eTmp - eOld) > 0.00001)
                                                                                {
                                                                                        cerr << "# (dbg main#" << phase << ") energy is different. iterative: " << eOld << "; actual: " << eTmp << endl;
                                                                                }
                                                                                */
                                                                        }

                                                                        if (config.isRestart() && (eOld - statData.lowerEnergy) < -statData.deltaEnergy) // if found lower energy
                                                                        {
                                                                                #pragma omp critical
                                                                                {
                                                                                        statData.foundLowerEnergy = 1;
                                                                                        statData.lowerEnergy = eOld;
                                                                                        statData.lowerEnergyState = sys.state.toString();
                                                                                        statData.temperatureOfLowerEnergy = tt;
                                                                                }
                                                                        }
                                                                }
                                                        }

                                                        /*
                                                        #pragma omp critical
                                                        {
                                                                std::cout << "phase: " << phase << std::endl;
                                                        }
                                                        */
                                                        
                                                }
                                        }



                                        // phase=0 is the heatup, phase=1 is calculate
                                        // full recalculte energy
                                        eOld = sys.E();
                                        // add external field
                                        for (auto p : sys.parts)
                                        {
                                                eOld -= p->m.scalar(field);
                                        }


                                        for (auto &cp : calculationParameters)
                                        {
                                                cp->init(&sys); // attach the system and calculate the init value
                                        }
                                        
                                        unsigned calculateSteps = swapConst;

                                        for (unsigned step = 0; step < calculateSteps; ++step)
                                        {
                                                //std::cout << "all works " << std::endl;
                                                /*
                                                if (omp_get_thread_num() == 0)
                                                {
                                                        std::cout << "sys energy: " << sys.E() << std::endl;
                                                }
                                                */
                                                // full recalculte energy every to avoid FP error collection
                                                if (step != 0 && step % FULL_REFRESH_EVERY == 0)
                                                {
                                                        eOld = sys.E();
                                                        // add external field
                                                        for (auto p : sys.parts)
                                                        {
                                                                eOld -= p->m.scalar(field);
                                                        }

                                                        if (statData.foundLowerEnergy){
                                                                //cancel the calculations
                                                                break; //break up the main for loop
                                                        }
                                                }

                                                for (unsigned sstep = 0; sstep < N; ++sstep)
                                                {

                                                        dE = 0;
                                                        swapNum = intDistr(generator);
                                                        Part *partA = sys.getById(swapNum);

                                                        { // get dE
                                                                unsigned j = 0;

                                                                if (sys.interactionRange() != 0.0)
                                                                {
                                                                        for (Part *neigh : sys.neighbours[swapNum])
                                                                        {
                                                                                if (neigh->state == partA->state) // assume it is rotated, inverse state in mind
                                                                                        dE -= 2. * sys.eAt(swapNum, j);
                                                                                else
                                                                                        dE += 2. * sys.eAt(swapNum, j);
                                                                                ++j;
                                                                        }
                                                                }
                                                                else
                                                                {
                                                                        for (Part *neigh : sys.parts)
                                                                        {
                                                                                if (partA != neigh)
                                                                                {
                                                                                        if (neigh->state == partA->state)
                                                                                                dE -= 2. * sys.eAt(swapNum, j);
                                                                                        else
                                                                                                dE += 2. * sys.eAt(swapNum, j);
                                                                                        ++j;
                                                                                }
                                                                        }
                                                                }

                                                                dE += 2 * partA->m.scalar(field);
                                                        }

                                                        acceptSweep = false;
                                                        if (dE < 0 || t == 0)
                                                        {
                                                                acceptSweep = true;
                                                        }
                                                        else
                                                        {
                                                                p = exp(-dE / t);
                                                                randNum = doubleDistr(generator);
                                                                if (randNum <= p)
                                                                {
                                                                        acceptSweep = true;
                                                                }
                                                        }

                                                        if (acceptSweep)
                                                        {
                                                                sys.parts[swapNum]->rotate(false);
                                                                eOld += dE;

                                                                for (auto &cp : calculationParameters)
                                                                {
                                                                        cp->iterate(partA->Id());
                                                                }

                                                                if (config.debug)
                                                                {
                                                                        // recalc energy
                                                                        double eTmp = sys.E();

                                                                        // add external field
                                                                        for (auto pt : sys.parts)
                                                                        {
                                                                                eTmp -= pt->m.scalar(field);
                                                                        }
                                                                        /*
                                                                        if (fabs(eTmp - eOld) > 0.00001)
                                                                        {
                                                                                cerr << "# (dbg main#" << phase << ") energy is different. iterative: " << eOld << "; actual: " << eTmp << endl;
                                                                        }
                                                                        */
                                                                }

                                                                if (config.isRestart() && (eOld - statData.lowerEnergy) < -statData.deltaEnergy) // if found lower energy
                                                                {
                                                                        #pragma omp critical
                                                                        {
                                                                                statData.foundLowerEnergy = 1;
                                                                                statData.lowerEnergy = eOld;
                                                                                statData.lowerEnergyState = sys.state.toString();
                                                                                statData.temperatureOfLowerEnergy = tt;
                                                                        }
                                                                }
                                                        }
                                                }

                                                // update thermodynamic averages (porosyenok ;)
                                                e += eOld;
                                                e2 += eOld * eOld;
                                                e_array[tt] = e.get_d();
                                                /*
                                                #pragma omp critical
                                                {
                                                                std::cout << "phase: " << phase << std::endl;
                                                }
                                                */
                                                for (auto &cp : calculationParameters)
                                                {
                                                        cp->incrementTotal();
                                                }

                                                if (config.getSaveStates()>0 && step % config.getSaveStates() == 0){
                                                        sys.save( config.getSaveStateFileName(tt,step) );
                                                }
                                                
                                                /*
                                                #pragma omp critical
                                                {
                                                                std::cout << "phase: " << phase << std::endl;
                                                }
                                                */
                                                if (step == (swapConst-1))
                                                {
                                                        #pragma omp critical
                                                        {
                                                                e_tt_array[tt] += e.get_d()/swapConst;
                                                                e_tt_array_squared[tt] += e2.get_d()/swapConst;
                                                        }
                                                }
                                        }
                                }


                                
				PartArray sys(config.getSystem());
                                s_array[tt] = sys.state.toString();
				/*
				#pragma omp critical
				{
					std::cout << e_tt_array[tt]/(m_ + 1) << " ";
				}
				std::cout << std::endl;
				*/
                        }
                }
                if (flag == true)
                {
                        #pragma omp barrier

                        //s_array[tt] = sys.state.toString();

                        if (omp_get_thread_num() == 0)
                        {
                                //#pragma omp master
                                std::cout << "Start swapping: " << std::endl;
                                for (int j_ = config.temperatures.size() - 2; j_ > -1; j_--)
                                {
                                        double p = std::pow(2.718282, ((e_array[j_ + 1]/swapConst - e_array[j_]/swapConst) * (1 / t_array[j_ + 1] - 1 / t_array[j_])));
                                        std::cout << j_ + 1 << " energy: " << e_array[j_+1]/swapConst << "; " << j_ << "energy: " << e_array[j_]/swapConst << "; ";
                                        std::cout << j_ << " probability: " << p << " ";
                                        if (urd(gen) > p)
                                        {
                                                std::string tmp__ = s_array[j_+1];
                                                s_array[j_ + 1] = s_array[j_];
                                                s_array[j_] = tmp__;

                                                probabilities[m_] += 1;
                                        }
                                }
                                std::cout << std::endl;

                                for (int j_ = 0; j_ < config.temperatures.size(); j_++)
                                {
                                        config.applyState(s_array[j_]);
                                }
                                PartArray sys(config.getSystem());
                        }
                }
        }

	std::vector<double> cT (temperatureCount);
	PartArray sys(config.getSystem());
	std::cout<< "All system works:" << std::endl;

        #pragma omp parallel
        {
                #pragma omp for
                for (int tt = 0; tt < config.temperatures.size(); ++tt)
                {
                        //PartArray sys(config.getSystem());
			std::vector<std::unique_ptr<CalculationParameter>> calculationParameters;
			/*
                        e_tt_array[tt] /= config.getCalculate();
                        e_tt_array_squared[tt] /= config.getCalculate();
			*/
                        cT[tt] = (e_tt_array_squared[tt]/m_steps - ((e_tt_array[tt]/m_steps) * (e_tt_array[tt]/m_steps))) / (config.temperatures[tt] * config.temperatures[tt] * sys.size());
                        /*
                        out_c.open("Capacities.txt", fstream::app);
                        if (out_c.is_open())
                        {
                                out_c << cT.get_mpf_t();
                                out_c << "\n";
                        }
                        out_c.close();
                        */
                        statData.finalStates[tt] = sys.state.toString();
                        statData.finalEnergies[tt] = e_tt_array[tt]/m_steps;
                        statData.temperature_times_end[tt] = std::chrono::steady_clock::now();

                        #pragma omp critical
                        {
                                std::cout << "debug works 2" << std::endl;
                                /* //
                                gmp_printf("%e %.30Fe %.30Fe %.30Fe %d %d",
                                                t, cT.get_mpf_t(), e.get_mpf_t(), e2.get_mpf_t(),
                                                omp_get_thread_num(), trseed);
                                */ //
                                out_data.open("Apamea_output.txt", fstream::app);
                                if (out_data.is_open())
                                {

                                                out_data << config.temperatures[tt] << " " << cT[tt] << " " << e_tt_array[tt]/m_steps << " " <<e_tt_array_squared[tt]/m_steps;
                                                out_data << "\n";
                                }
                                out_data.close();
                                /* //
                                for (auto &cp : calculationParameters)
                                {
                                        gmp_printf(" %.30Fe %.30Fe",
                                                        cp->getTotal(config.getCalculate()).get_mpf_t(),
                                                        cp->getTotal2(config.getCalculate()).get_mpf_t());
                                }
                                */ //
                                auto rtime = std::chrono::duration_cast<std::chrono::milliseconds>(statData.temperature_times_end[tt] - statData.temperature_times_start[tt]).count();
                                //printf(" %f", rtime / 1000.);
                                //printf("\n");
                                fflush(stdout);
                                for (auto &cp : calculationParameters)
                                {
                                        cp->save(tt);
                                }
                        }
                }
        }

        return statData;
}

int main(int argc, char *argv[])
{
        std::ofstream out_c;
        out_c.open("Capacities.txt");
        out_c.close();

        std::ofstream out_data;
        out_data.open("Apamea_output.txt");
        out_data.close();
        //  #1:T 2:C(T)/N 3:<E> 4:<E^2>
        out_data.open("Apamea_output.txt", fstream::app);
        if (out_data.is_open())
        {
                out_data << "#1:T 2:C(T)/N 3:<E> 4:<E^2>";
                out_data << "\n";
        }
        out_data.close();

        auto time_start = std::chrono::steady_clock::now();

        auto config = readParameters(argc,argv);
        if (!config){
                return 0;
        }

        int k = 1;
        int D = 1;
        int swapConst = 10000;
        std::vector<double> probabilities (config->getCalculate()/swapConst);
	for (int i = 0; i < probabilities.size(); i++) probabilities[i] = 0;

        bool programRestarted = false;
        monteCarloStatistics statData;
        std::string finalState = config->getSystem().state.toString();
        do {
                //ConfigManager data_temperatures = config;
                int flag = false;
                //std::cout << "Temperatures balancing started: " << i+1 <<"/" << k << " ";
                //std::cout << std::endl;

                std::cout<<"start balancing: " << std::endl;
/* //
                for (int i = 0; i < k; i++)
                {
                        std::cout<<"balancing step: " << i+1 <<"/" << k << std::endl;
                        statData = montecarlo(*config, flag, swapConst, probabilities); // Р В·Р В°Р С—РЎС“РЎРѓР С” РЎРѓР В°Р СР С‘РЎвЂ¦ Р Р†РЎвЂ№РЎвЂЎР С‘РЎРѓР В»Р ВµР Р…Р С‘Р в„–
                        config->temperatures = interpolation_temperatures_(config->temperatures, statData.finalEnergies, config->temperatures.size());
                }
                //std::mt19937 gen((int)time(0));
                //std::uniform_real_distribution<> urd(0., 1.);
*/ //
                flag = true;
                std::cout<< "balancing executed. Start default tempering: " << std::endl;
                for (int i = 0; i < D; i++)
                {
                        std::cout<<"tempering step: " << i+1 <<"/" << D << std::endl;
                        statData = montecarlo(*config, flag, swapConst, probabilities); // Р В·Р В°Р С—РЎС“РЎРѓР С” РЎРѓР В°Р СР С‘РЎвЂ¦ Р Р†РЎвЂ№РЎвЂЎР С‘РЎРѓР В»Р ВµР Р…Р С‘Р в„–
                }


                std::vector<double>p_statistics (config->getCalculate()/swapConst);
		for (int i = 0; i < config->getCalculate()/swapConst; i++) std::cout << probabilities[i] << " ";
		std::cout << std::endl;

                for (int i = 0; i < config->temperatures.size()-1; i++)
                {
                        probabilities[i] = (probabilities[i]/(config->getCalculate()/swapConst))*100;
                        std::cout << probabilities[i] << " ";
                }
                std::cout << std::endl;

        /*
                if (statData.foundLowerEnergy){
                        config->applyState(statData.lowerEnergyState);
                        printf("# -- restart MC: found lower energy %g < %g, at T%d=%g new state: %s\n",
                           statData.lowerEnergy,
                           statData.initEnergy,
                           statData.temperatureOfLowerEnergy,
                           config->temperatures[statData.temperatureOfLowerEnergy],
                           statData.lowerEnergyState.c_str());
                        programRestarted = true;
                        finalState = xorstr(finalState,statData.lowerEnergyState);
                }
        */
        statData.foundLowerEnergy = false;
        } while(statData.foundLowerEnergy);

        auto time_end = std::chrono::steady_clock::now();

        // print out the states and times of running
        int64_t time_proc_total = 0;
        printf("###########  end of calculations #############\n");
        printf("#\n");
        printf("###########     final notes:     #############\n");
        for (int tt = 0; tt < config->temperatures.size(); ++tt)
        {
                auto rtime = std::chrono::duration_cast<std::chrono::milliseconds>(statData.temperature_times_end[tt] - statData.temperature_times_start[tt]).count();
                printf("#%d, time=%fs, T=%e, E=%e, final state: %s\n",
                           tt,
                           rtime / 1000.,
                           config->temperatures[tt],
                           statData.finalEnergies[tt],
                           statData.finalStates[tt].c_str());
                time_proc_total += rtime;
        }

        printf("#\n");
        int64_t time_total = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
        double speedup = double(time_proc_total) / time_total;
        printf("# total time: %fs, speedup: %f%%, efficiency: %f%%\n", time_total / 1000., speedup * 100, speedup / config->threadCount * 100);
/*
        if (programRestarted){
                printf("\n##### Warning! The program was restarted because it found the lower energy.\n");
                printf("##### But the console output before this moment can not be wiped!\n");
                printf("##### Remove all the result lines before the last line starting with:\n");
                printf("# -- restart MC:\n");
                //printf("# Command to delete this lines:\n");
                //printf("#    perl -p0e -i 's/(# 1:T[^\\n]+\\n).+# -- restart MC: found[^\\n]+\\n/$1/s'   <filename>\n#\n");

                printf("# configuration of the lowest energy: %s\n",finalState.c_str());
                if (!config->getNewGSFilename().empty()){
                        config->saveSystem(config->getNewGSFilename());
                        printf("# system with found lowest energy is saved to file %s\n",config->getNewGSFilename().c_str());
                }
        }
*/
        std::ofstream out;

        out.open("Temperatures.txt");
        if (out.is_open())
        {
                for (double x : config->temperatures) out << x << " ";
                out << "\n";
        }
        out.close();

        printf("Program executed");

        return 0;
}
