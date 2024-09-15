#pragma once
#include <unordered_map>
#include <string>
#include <deque>
#include <memory>
#include <set>

/* 
Fencer struct
Represents a fencer
Stores the name of the fencer
Stores the names of fencers it is already in queue with 
*/
struct Fencer
{
public:
	Fencer() {}
	Fencer(std::string name) : Name(name) {}

	std::string Name;
	std::unordered_map<std::string, Fencer> pairedFencers;
	int QueueCooldownCounter = 0;

	// Function returns true if Fencer1 is already paired with given fencerName
	bool findPairedFencer(std::string name)
	{
		if (pairedFencers.find(name) != pairedFencers.end())
		{
			return true;
		}
		return false;
	}
};

struct findSingleResult
{
	bool result;
	std::string message;
};

/*
StripQueue class, works on a first-come first-serve basis
Fencers can be added to the queue as singles or pairs
Sigining up as a pair must be done with another fencer beforehand
Single fencers will be paired with the next single fencer in queue
*/
class StripQueue
{
public:
	StripQueue();

	// TODO: Change return type to string to send result messages to user
	std::string AddFencerSingle(std::string newFencerName);
	std::string AddFencerPair(std::string newFencerName_1, std::string nswitewFencerName_2);

	std::string PrintQueue();
	std::string nextBout();

	std::string addToMap(std::string newFencerName);
	std::string findInMap(std::string newFencerName);

	// Hash table containing the names of every fencer currently in queue
	std::unordered_map<std::string, Fencer> m_QueuedFencers;

	// Dev Populate Queue
	void PopulateQueue();

private:
	// Queue containing pairs of fencers
	std::deque<std::pair<Fencer*, Fencer*>> m_StripQueue;

	// Helpers
	// ------------------------------
	// Single Fencer
	bool m_singleExists = false;
	findSingleResult FindSingle(Fencer& newFencer);
	std::pair<Fencer*, Fencer*> SkipSingleFencer();

	// Quick queue prevention
	bool TooSoon(std::string FencerName);
	void AddToCooldown(std::string FencerName);
	void ReduceCooldowns();
	std::unordered_map<std::string, Fencer> m_coolDownFencers;

	// Repeeat opponent check
	bool RepeatOpponent(std::string FencerName, std::string OpponentName);
};
