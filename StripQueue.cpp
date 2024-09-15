#include "StripQueue.h"
#include <iostream>
using namespace std;

StripQueue::StripQueue()
{
}

// Add a single fencer to the queue
// Works as a way to join the queue and adds your name to a map of fencers
// Will be matched up with the next person to sign up as a single
string StripQueue::AddFencerSingle(std::string newFencerName)
{
	if (TooSoon(newFencerName))
	{
		return "[" + newFencerName + "] cannot sign up so soon after being added to the queue.";
	}

	// If no other single fencers are in the queue
	if (!m_singleExists)
	{
		if (m_QueuedFencers.find(newFencerName) != m_QueuedFencers.end())
		{
			m_StripQueue.push_back(std::make_pair(&m_QueuedFencers[newFencerName], nullptr));
			m_singleExists = true;

			ReduceCooldowns();
			AddToCooldown(newFencerName);
			return "[" + newFencerName + "]" + "has been added to the queue successfully!";
		}
		else
		{
			Fencer newFencer(newFencerName);
			m_QueuedFencers[newFencerName] = newFencer;
			m_StripQueue.push_back(std::make_pair(&m_QueuedFencers[newFencerName], nullptr));
			m_singleExists = true;

			ReduceCooldowns();
			AddToCooldown(newFencerName);
			return "[" + newFencerName + "]" + " has been added to the queue successfully!";
		}
	}

	// If a single fencer is already in the queue, pair them up
	else
	{
		if (m_QueuedFencers.find(newFencerName) != m_QueuedFencers.end())
		{
			// Check if there is a single fencer
			// Cannot be paired with themselves
			findSingleResult singleRes= FindSingle(m_QueuedFencers[newFencerName]);
			if (!singleRes.result)
				return singleRes.message;

			m_singleExists = false;

			AddToCooldown(newFencerName);
			return "[" + newFencerName + "]" + " has been added to the queue successfully!";
		}
		else
		{
			Fencer newFencer(newFencerName);
			m_QueuedFencers[newFencerName] = newFencer;

			// Check if there is a single fencer
			// Cannot be paired with themselves
			findSingleResult singleRes = FindSingle(m_QueuedFencers[newFencerName]);
			if (!singleRes.result)
				return singleRes.message;

			m_singleExists = false;

			AddToCooldown(newFencerName);
			return "[" + newFencerName + "]" + " has been added to the queue successfully!";
		}
	}
	
	return newFencerName + " was not added to the queue, please try again.";
}


// Helper function to find the single fencer in a queue
findSingleResult StripQueue::FindSingle(Fencer& newFencer)
{
	findSingleResult result;
	for (auto& queueElement : m_StripQueue)
	{
		if (queueElement.second == nullptr)
		{
			if (queueElement.first->Name == newFencer.Name)
			{
				result.message = "[" + newFencer.Name + "]" + " is still waiting for an opponent and cannot be added to the queue again.";
				result.result = false;
				return result;
			}
			if (RepeatOpponent(queueElement.first->Name, newFencer.Name))
			{
				result.message = "[" + newFencer.Name + "] has already fenced [" + queueElement.first->Name + "] and have been paired again."
					"please join as pair a pair to fence each other again ";
				result.result =  false;
				return result;
			}

			queueElement.second = &newFencer;
			result.result = true;
			return result;
		}
	}

	result.result = false;
	return result;

}

// Add a premade pair of fencers to the queue
// If either fencer has signed up too recently, they will not be added
// If a fencer is already in the queue, they will be paired with the new fencer
string StripQueue::AddFencerPair(std::string newFencerName_1, std::string newFencerName_2)
{

	// Check if the fencers are the same
	// Check if either fencer has signed up too recently
	if (newFencerName_1 == newFencerName_2)
	{
		return "Fencer can not join the queue with themselves.";
	}
	else if (TooSoon(newFencerName_1)) 
	{
		return "[" + newFencerName_1 + "] cannot sign up so soon after being added to the queue.";
	}
	else if (TooSoon(newFencerName_2))
	{
		return "[" + newFencerName_2 + "] cannot sign up so soon after being added to the queue.";
	}


	// All cases for successfuly entry in to the queue
	else if (m_QueuedFencers.find(newFencerName_1) != m_QueuedFencers.end() && m_QueuedFencers.find(newFencerName_2) != m_QueuedFencers.end())
	{
		m_StripQueue.push_back(std::make_pair(&m_QueuedFencers[newFencerName_1], &m_QueuedFencers[newFencerName_2]));
		
		ReduceCooldowns();
		AddToCooldown(newFencerName_1);
		AddToCooldown(newFencerName_2);
		return "[" + newFencerName_1 + "] and [" + newFencerName_2 + "] have been added to the queue successfully!";
	}
	else if (m_QueuedFencers.find(newFencerName_1) != m_QueuedFencers.end() && m_QueuedFencers.find(newFencerName_2) == m_QueuedFencers.end())
	{
		Fencer newFencer(newFencerName_2);
		m_QueuedFencers[newFencerName_2] = newFencer;
		m_StripQueue.push_back(std::make_pair(&m_QueuedFencers[newFencerName_1], &m_QueuedFencers[newFencerName_2]));

		ReduceCooldowns();
		AddToCooldown(newFencerName_1);
		AddToCooldown(newFencerName_2);
		return "[" + newFencerName_1 + "] and [" + newFencerName_2 + "] have been added to the queue successfully!";
	}
	else if (m_QueuedFencers.find(newFencerName_1) == m_QueuedFencers.end() && m_QueuedFencers.find(newFencerName_2) != m_QueuedFencers.end())
	{
		Fencer newFencer(newFencerName_1);
		m_QueuedFencers[newFencerName_1] = newFencer;
		m_StripQueue.push_back(std::make_pair(&m_QueuedFencers[newFencerName_1], &m_QueuedFencers[newFencerName_2]));
		
		ReduceCooldowns();
		AddToCooldown(newFencerName_1);
		AddToCooldown(newFencerName_2);
		return "[" + newFencerName_1 + "] and [" + newFencerName_2 + "] have been added to the queue successfully!";
	}
	else
	{
		Fencer newFencer1(newFencerName_1);
		Fencer newFencer2(newFencerName_2);
		m_QueuedFencers[newFencerName_1] = newFencer1;
		m_QueuedFencers[newFencerName_2] = newFencer2;
		m_StripQueue.push_back(std::make_pair(&m_QueuedFencers[newFencerName_1], &m_QueuedFencers[newFencerName_2]));
		
		ReduceCooldowns();
		AddToCooldown(newFencerName_1);
		AddToCooldown(newFencerName_2);
		return "[" + newFencerName_1 + "] and [" + newFencerName_2 + "] have been added to the queue successfully!";
	}

	return "[" + newFencerName_1 + "] and [" + newFencerName_2 + "] have been added to the queue successfully!";
}

// Returns a string stating the two fencers bouting next
// Removes the pair from the queue
std::string StripQueue::nextBout()
{
	std::string boutOutput;
	if (m_StripQueue.empty())
	{
		return "No fencers in queue";
	}
	else
	{
		std::pair<Fencer*, Fencer*> onStrip = m_StripQueue.front();
		if (onStrip.second == nullptr && m_StripQueue.size() > 1)
		{
			// Skip the single fencer
			onStrip = SkipSingleFencer();
			m_StripQueue.pop_front();
			
			// Add the opponents to each other's maps
			onStrip.first->pairedFencers[onStrip.second->Name] = *onStrip.second;
			onStrip.second->pairedFencers[onStrip.first->Name] = *onStrip.first;
			return (std::string)onStrip.first->Name + " vs " + (std::string)onStrip.second->Name;
		}
		else if (onStrip.second == nullptr && m_StripQueue.size() <= 1)
		{
			return "Only 1 fencer in queue";
		}
		else
		{
			m_StripQueue.pop_front();
			onStrip.first->pairedFencers[onStrip.second->Name] = *onStrip.second;
			onStrip.second->pairedFencers[onStrip.first->Name] = *onStrip.first;
			return std::string(onStrip.first->Name + " vs " + onStrip.second->Name);
		}
	}

	return boutOutput;
}

// If a single fencer is in front of the line:
// Skip and the next pair after them fences
// Place the single fencer at the front of the queue
std::pair<Fencer*, Fencer*> StripQueue::SkipSingleFencer()
{
	std::pair<Fencer*, Fencer*> singlePair = m_StripQueue.front();
	m_StripQueue.pop_front();
	std::pair<Fencer*, Fencer*> newPair = m_StripQueue.front();
	m_StripQueue.pop_front();
	m_StripQueue.push_front(singlePair);
	m_StripQueue.push_front(newPair);

	return newPair;
}

// Will return true if fencer cannot sign up twice
// Returns false if the fencer can sign up
bool StripQueue::TooSoon(std::string FencerName)
{
	if (m_coolDownFencers.find(FencerName) != m_coolDownFencers.end())
	{
		return true;
	}

	return false;
}

// Add fencer to the cooldown list
// Cooldown timer depends on the size of the queue
void StripQueue::AddToCooldown(std::string FencerName)
{
	int cooldown;
	if (m_QueuedFencers.size() < 4)
	{
		cooldown = 1;
	}
	else
	{
		cooldown = 2;
	}

	if (m_coolDownFencers.find(FencerName) == m_coolDownFencers.end())
	{
		m_coolDownFencers[FencerName] = m_QueuedFencers[FencerName];
		m_coolDownFencers[FencerName].QueueCooldownCounter = cooldown;	
	}
}

// Reduce the cooldown of all fencers in the cooldown list
// If a fencer's cooldown reaches 0, remove them from the list
void StripQueue::ReduceCooldowns()
{
	for (auto it = m_coolDownFencers.begin(); it != m_coolDownFencers.end(); )
	{
		if (it->second.QueueCooldownCounter > 0)
		{
			it->second.QueueCooldownCounter--;
		}

		// If the cooldown counter reaches 0, erase the element and move the iterator forward
		if (it->second.QueueCooldownCounter <= 0)
		{
			it = m_coolDownFencers.erase(it);  // Erase returns the next valid iterator
		}
		else
		{
			++it;  // Only move to the next element if nothing was erased
		}
	}
}


// Print the queue
string StripQueue::PrintQueue()
{
	std::string queueOutput;

	if (m_StripQueue.empty())
	{
		return "No fencers in queue.";
	}

	for (int i = 0; i < m_StripQueue.size(); i++)
	{
		if (m_StripQueue[i].second == nullptr)
		{
			queueOutput += "[" + m_StripQueue[i].first->Name + "] is waiting for an opponent.\n";
		}
		else
		{
			queueOutput += "[" + m_StripQueue[i].first->Name + "] vs [" + m_StripQueue[i].second->Name + "]\n";
		}
	}

	return queueOutput;
}

// Helper function needed for those wanting to use "Join as Pair" button
// Add a fencer's name to the map
std::string StripQueue::addToMap(std::string newFencerName)
{
	if (m_QueuedFencers.find(newFencerName) != m_QueuedFencers.end())
	{
		return "[" + newFencerName + "]" + "already in the list.";
	}
	else
	{
		Fencer newFencer(newFencerName);
		m_QueuedFencers[newFencerName] = newFencer;
		return "[" + newFencerName + "]" + " successfully added to the list.";
	}	
}

// Helper function for "Join as Pair" button to pair with another fencer
// If the user is in the map, return that Fencer's name
std::string StripQueue::findInMap(std::string newFencerName)
{
	if (m_QueuedFencers.find(newFencerName) != m_QueuedFencers.end())
	{
		return newFencerName;
	}
	else
	{
		return newFencerName + " is not in the list.";
	}
}

// Dev populate queue
void StripQueue::PopulateQueue()
{
	Fencer fencer1("Fencer1");
	Fencer fencer2("Fencer2");
	Fencer fencer3("Fencer3");
	Fencer fencer4("Fencer4");
	Fencer fencer5("Fencer5");
	Fencer fencer6("Fencer6");
	Fencer fencer7("Fencer7");
	Fencer fencer8("Fencer8");

	m_QueuedFencers["Fencer1"] = fencer1;
	m_QueuedFencers["Fencer2"] = fencer2;
	m_QueuedFencers["Fencer3"] = fencer3;
	m_QueuedFencers["Fencer4"] = fencer4;
	m_QueuedFencers["Fencer5"] = fencer5;
	m_QueuedFencers["Fencer6"] = fencer6;
	m_QueuedFencers["Fencer7"] = fencer7;
	m_QueuedFencers["Fencer8"] = fencer8;
}

bool StripQueue::RepeatOpponent(std::string fencer1, std::string fencer2)
{
	if (m_QueuedFencers.find(fencer1) != m_QueuedFencers.end() && m_QueuedFencers.find(fencer2) != m_QueuedFencers.end())
	{
		if (m_QueuedFencers[fencer1].pairedFencers.find(fencer2) != m_QueuedFencers[fencer1].pairedFencers.end())
		{
			return true;
		}
	}

	return false;
}

// **TODO: Implement a way to remove fencers from the queue**
// **TODO: Implement a way to turn the queue in to a random queue**
