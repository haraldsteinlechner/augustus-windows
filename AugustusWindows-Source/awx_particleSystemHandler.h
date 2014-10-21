class awx_particleSystem;

class awx_particleSystemHandler
{
	public:
	
	vector<awx_particleSystem*> systems;
	
	void addParticleSystem(awx_particleSystem *sys)
	{
		systems.push_back(sys);
	}
	
	void update();
};