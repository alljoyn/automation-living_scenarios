#include <stdio.h>
#include <qcc/Debug.h>
#include <datadriven/datadriven.h>
#include <LivingScenarioProxy.h>
#include <alljoyn/Init.h>

using namespace gen::org_alljoyn;


class LivingScenarioListener :
    public datadriven::Observer<LivingScenarioProxy>::Listener 
{
  public:
    void OnUpdate(const std::shared_ptr<LivingScenarioProxy>& scenario)
    {
	std::cout << "Updated" <<  std::endl;
    }

    void OnRemove(const std::shared_ptr<LivingScenarioProxy>& scenario)
    {
	std::cout << "Removed" <<  std::endl;
    }
};

class LivingScenarioSignalListener:
	public datadriven::SignalListener<LivingScenarioProxy, LivingScenarioProxy::LivingScenario> 
{
	void OnSignal(const LivingScenarioProxy::LivingScenario& signal)
	{
		const LivingScenarioProxy::Type::parameters_ss parameters = signal.parameters;
		std::cout << "Received the signal " << signal.name.c_str() << " with parameters:\n";

		for(auto const &it : parameters) {

			std::cout << "{ '" << it.first << "' : '" << it.second << "' }\n";
			

		}

	}
};


static void Finalize()
{
#ifdef ROUTER
    AllJoynRouterShutdown();
#endif
    AllJoynShutdown();
}

int main(int argc, char** argv)
{

	
	std::cout << "Press q then Enter to quit\n";

	if (AllJoynInit() != ER_OK) {
		return EXIT_FAILURE;
	}


#ifdef ROUTER
	if (AllJoynRouterInit() != ER_OK) {
		AllJoynShutdown();
		return EXIT_FAILURE;
	}
#endif

	LivingScenarioListener listener;
	std::shared_ptr<datadriven::Observer<LivingScenarioProxy>> observer = datadriven::Observer<LivingScenarioProxy>::Create(&listener);

	if(nullptr == observer)
	{
		std::cerr << "Couldn't initialize the observer\n";
		observer.reset();
		return EXIT_FAILURE;
	}

	std::cout << "Observer created\n";

	LivingScenarioSignalListener signalListener;

	if(ER_OK != observer->AddSignalListener<LivingScenarioProxy::LivingScenario>(signalListener))
	{
		std::cerr << "Failed to add signal listener\n";
		observer.reset();
		Finalize();
		return EXIT_FAILURE;
	}

	std::cout << "Signal listener added\n";


	while(true)
	{
		if(getchar() == 'q')
		{
			break;
		}
	}

	observer.reset();
	Finalize();

	return EXIT_SUCCESS;
}
