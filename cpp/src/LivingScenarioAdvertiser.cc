#include <stdio.h>
#include <qcc/Debug.h>
#include <LivingScenarioInterface.h>
#include <alljoyn/Init.h>

using namespace gen::org_alljoyn;

class _LivingScenario : 
	public datadriven::ProvidedObject, 
	public LivingScenarioInterface
{
private:
    pthread_mutex_t mutex;

public:
	_LivingScenario(std::shared_ptr<datadriven::ObjectAdvertiser> advertiser, const qcc::String& path="") :
		ProvidedObject(advertiser, path),
		LivingScenarioInterface(this),
		mutex(PTHREAD_MUTEX_INITIALIZER)
	{
		
	}

	QStatus Prepare()
	{

		QStatus status = GetStatus();

		if(ER_OK == status) 
		{
			pthread_mutex_lock(&mutex);
			status = PutOnBus();
			pthread_mutex_unlock(&mutex);
		}

		
		return status;
		
	}

	QStatus SendLivingScenario(const qcc::String& name)
	{
		QStatus status = ER_OK;
		Type::parameters_ss params;


		params["key"] = "value";

		pthread_mutex_lock(&mutex);
		status = LivingScenario(name, params);
		pthread_mutex_unlock(&mutex);

		return status;
	}
};
	
void usage()
{
	printf("Wrong number of arguments!\nUsage: LivingScenarioAdvertiser [scenario name]\n");
}

static void Finalize()
{
#ifdef ROUTER
    AllJoynRouterShutdown();
#endif
    AllJoynShutdown();
}


int main(int argc, char** argv)
{

	if(argc < 1)
	{
		usage();
		return -1;
	}

	if (AllJoynInit() != ER_OK) {
		return EXIT_FAILURE;
	}


#ifdef ROUTER
	if (AllJoynRouterInit() != ER_OK) {
		AllJoynShutdown();
		return EXIT_FAILURE;
	}
#endif

	std::shared_ptr<datadriven::ObjectAdvertiser> advertiser = datadriven::ObjectAdvertiser::Create();

	if(nullptr == advertiser)
	{
		std::cerr << "Object advertiser couldn't be initialized\n";
		Finalize();
		return EXIT_FAILURE;
	}

	qcc::String name(argv[1]);

	qcc::String path("/LivingScenario");

	std::unique_ptr<_LivingScenario> livingScenario = std::unique_ptr<_LivingScenario>(new _LivingScenario(advertiser, path));
	
	QStatus status = ER_OK;


	livingScenario->Prepare();
	
	sleep(1);
	//printf("Living scenario put on bus: %s\n", QCC_StatusText(status));
	status = livingScenario->SendLivingScenario(name);

	

	if(ER_OK != status)
	{
		std::cerr << "Couldn't send the living scenario signal: " << QCC_StatusText(status) << std::endl;
		advertiser.reset();
		Finalize();
		return EXIT_FAILURE;
	}

	std::cout << "Signal successfully sent\n";


	advertiser.reset();

	Finalize();

	return EXIT_SUCCESS;
}
