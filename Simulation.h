#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <QtCore>



//#define CLEANROOM

//comment out if you want debug mode  -----start
#ifndef CLEANROOM

	#define VARIED_ENERGY //does not give the whole playing field the same energy
	//#define DECREASE_ENERGY //decreases energy over time
	//#define EXECUTION_ERRORS //creates random execution errors

	#define REPRODUCTION_ERRORS //mutates genome when reproducing
	#define DEAD_MUTATION //mutates dead cells

	#define RANDOM_INITIAL_CELLS

	#define DISASTERS

	//#define BAD_KILLS
	#define MUST_REPRODUCE //creatures get killed if they don't reproduce

#endif
//comment out if you want debug mode -------end

#define WORLD_X 640
#define WORLD_Y 480
#define WORLD_Z 1
#define GENOME_SIZE 100 //number of operations in a genome

#define GENOME_OPERATIONS 37 //number of different operations
#define NO_REP_OPERATION 11 //id of the NO REPRODUCE operation

#define EAT_ENERGY GENOME_SIZE //amount of energy gained from eating

#define LIVING 3 //minimum generation to be considered alive

#define MAX_EXECUTION_ROW 5 //how many cells can be executed in a row

#define DIRECTIONS 4 //change if you want 3d

#define NORTH 0
#define WEST 1
#define SOUTH 2
#define EAST 3
#define UP 4
#define DOWN 5

#define MUTATION_RATE_REPRODUCTION 2000
//#define MUTATION_RATE_EXECUTION 100000
//#define MUTATION_RATE_NON_LIVING 200
//#define MAX_MUTATIONS_NON_LIVING 3

#define ENERGY_ADDED 5000
#define ENERGY_FREQUENCY 25

#define REPRODUCTION_COST_FACTOR 1

#define ENERGY_DECREASE 5000000

#define ENERGY2_CONVERSION_GAIN 13

#define MIN_COPY 5

#define MAX_ENERGY 6000

#define DISASTER_CHANCE 5000000

#define SAVE_TIME 300000000 //+- 3 minutes with no disasters

#define LANDSCAPE_LINES 6

#define MAX_EXECUTING 1300

#define SPECIAL_COMMANDS 5 //number of allowed special commands like kill, move

enum Disasters
{
	Meteor,
	Poison,
	Hunger,
	Killer,
	Living,
	Flood
};

struct Cell{
	uint genome_size;
	uint genome_operations;
	qlonglong id;
	qlonglong parent;
	uint generation;
	uint energy;
	uint energy2;
	qlonglong lineage;
	uchar genome[GENOME_SIZE+1];
	uchar reproduced;
	uint bad;
	uint brain;
	struct Place *place;
	uint size;
	uchar facing;
	uint homePond;
};

struct Place{
	bool dead; //can we reproduce at this place?
};

struct Position{
	int x;
	int y;
	int z;
};

class Simulation: public QThread
{
	Q_OBJECT
public:
	Simulation(QQueue <struct Cell>*pool,QSemaphore *geneblocker,int id);
	virtual ~Simulation();
	void run();
	void stopIt(){running = false;};

	int x(){ return WORLD_X;}
	int y(){ return WORLD_Y;}
	int z(){ return WORLD_Z;}

	struct Cell *cell(int x, int y, int z);

	int genomeSize(){ return GENOME_SIZE;}

	void regenerateEnergy();
	void pause();
	void resume();
	uint counter(){return count;};
	void setEnergyAdd(uint value){ energyAdd = value;};
	uint getEnergyAdd(){return energyAdd;};
	uint getMaxEnergyAdd(){ return ENERGY_ADDED;};

	void saveWorld(QString file);
	void loadWorld(QString file);
	int id(){return myId;}
	int executed();
	void init();
	void addCell(uchar *genome, uint size);

private:
	struct Cell cells[WORLD_X][WORLD_Y][WORLD_Z];
	struct Place world[WORLD_X][WORLD_Y][WORLD_Z];
	unsigned long long cellid; //used to track new cells
	bool running; //stop exection when false
	QSemaphore *mutex;
	unsigned long long round; //total number of rounds
	uint count; //stat value

	uint energyAdd;

	inline uchar randomOperation();
	inline int randomX();
	inline int randomY();
	inline int randomZ();
	inline int randValue(int value);

	void executeCell(int x, int y, int z);
	void mutateCell(struct Cell *cell);
	void killCell(struct Cell *cell);
	bool reproduce(struct Cell *cell, struct Cell *neighbour,uchar *output_buffer);

	bool accessOk(struct Cell *source, struct Cell *dest, char guess,bool good);
	struct Position getNeighbour(int x, int y, int z, uchar direction);
	void disaster();

	int nextx;
	int nexty;
	int nextz;
	bool nextSet;
	int canExecuteNext;

	unsigned long mutated;
	bool initialized;
	int myId;

	QQueue <struct Cell>*genepool;
	QSemaphore *genepoolblocker;
	static const double randScale  = 1.0 / (1.0 + RAND_MAX);

	bool catas;

#ifdef Q_OS_WIN
	static const double randScaleBig  = 1.0 / (1.0 + 1073741824.0); //2^30
#else
	static const double randScaleBig  = 1.0 / (1.0 + RAND_MAX);
#endif

};

#endif /*SIMULATION_H_*/
