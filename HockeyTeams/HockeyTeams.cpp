#include <iostream>
#include <fstream>
#include <string>

/*
* Singly linked template list (container)
*/
template <typename T>
class ForwardList
{
	/*
	* Singly linked list node structure of data and a pointer to the next element
	*/
	struct Node
	{
		/* Data */
		T data;

		/* Pointer to next node */
		Node *next;
	};

	/* Pointer to head of singly linked list, nullptr by default */
	Node *head{};

public:

	/*
	* Singly linked list iterator class
	*/
	class ForwardListIterator
	{
		Node *node;

	public:
		ForwardListIterator(Node *const node) noexcept : node(node) {}

		/* Redefining the ++ operator, the iterator in this case goes to the next element */
		const ForwardListIterator &operator++() noexcept
		{
			node = node->next;
			return *this;
		}

		bool operator!=(const ForwardListIterator &forwardListIterator) const noexcept
		{
			return node != forwardListIterator.node;
		}

		bool operator==(const ForwardListIterator &forwardListIterator) const noexcept
		{
			return node == forwardListIterator.node;
		}

		/* Redefining the operator *, in this case, the iterator will be dereferenced */
		T &operator*() const noexcept
		{
			return node->data;
		}
		
		/* TODO */
		const T *operator->() const noexcept
		{
			return &node->data;
		}
	};

	/* Creating a Default Constructor */
	ForwardList() = default;

	/* Copy constructor implementation, needed to copy the contents of one linked list to another */
	ForwardList(const ForwardList &forward_list)
	{
		/* Copy content from forward_list */
		for (auto &&it : forward_list)
		{
			Add(it);
		}
	}

	/* Add to linked list method */
	void Add(const T &dt) noexcept
	{
		head = new Node{ dt, head };
	}

	/* Linked list removal method */
	void Del() noexcept
	{
		/* If the list is not empty */
		if (head)
		{
			/* then */
			auto tmp = head;
			head = head->next;
			delete tmp;
		}
	}

	/* Necessary for: range based for loop */
	ForwardListIterator begin() const noexcept
	{
		return head;
	}

	/* Necessary for: range based for loop */
	ForwardListIterator end() const noexcept
	{
		return nullptr;
	}

	/*
	* Destructor.
	* Required to clear the contents of a singly linked list.
	* Called when an object goes out of scope
	*/
	~ForwardList() noexcept
	{
		while (head)
		{
			Del();
		}
	}
};

/*
* Hockey team description structure
*/
struct HockeyTeam
{
	/* The names of the attackers are also stored in our singly linked list. */
	ForwardList<std::string> names_of_attackers;
	std::string team_name;

	int number_of_goals_scored;
};

class HockeyTeams
{
	ForwardList<HockeyTeam> hockey_teams;

public:
	HockeyTeams() = default;

	HockeyTeams(const ForwardList<HockeyTeam> &hockey_teams) : hockey_teams(hockey_teams) {}

	/*
	* The method returns an iterator in a singly linked list pointing to the command
	* found by the function by the name of the attacker,
	* otherwise an iterator to the next after the last element in the singly linked list.
	*/
	ForwardList<HockeyTeam>::ForwardListIterator SearchByLastName(const std::string &last_name) const noexcept
	{
		/* Cycle by command */
		for (auto hockey_team{ hockey_teams.begin() }; hockey_team != hockey_teams.end(); ++hockey_team)
		{
			/* Cycle by the names of attackers */
			for (auto &&attackers_name : hockey_team->names_of_attackers)
			{
				if (attackers_name == last_name)
				{
					return hockey_team;
				}
			}
		}

		return hockey_teams.end();
	}

	/*
	* The method that returns the command with the best statistics
	*/
	ForwardList<HockeyTeam>::ForwardListIterator MostScoringGame() const noexcept
	{
		auto max_value{ hockey_teams.begin() };

		for (auto hockey_team{ max_value }; hockey_team != hockey_teams.end(); ++hockey_team)
		{
			if (hockey_team->number_of_goals_scored > max_value->number_of_goals_scored)
			{
				max_value = hockey_team;
			}
		}

		return max_value;
	}

	/*
	* Serializing data about hockey teams to a file stream
	*/
	void Serialize(std::fstream &&file) const noexcept
	{
		for (auto &&it : hockey_teams)
		{
			for (auto &&it2 : it.names_of_attackers)
			{
				file << it2 + ' ';
			}

			/* Surnames of the attackers from the rest of the data are separated by a line */
			file << "| " + it.team_name + ' ' + std::to_string(it.number_of_goals_scored) + '\n';
		}
	}

	/*
	* Deserializing hockey team data from a file stream
	*/
	void Deserialize(std::fstream &&file) noexcept
	{
		std::string tmp;

		while (file >> tmp)
		{
			ForwardList<std::string> names_of_attackers;

			while (tmp != "|")
			{
				names_of_attackers.Add(tmp);
				file >> tmp;
			}

			HockeyTeam hockey_team{ .names_of_attackers = names_of_attackers };
			file >> hockey_team.team_name;
			file >> tmp; hockey_team.number_of_goals_scored = std::stoi(tmp);

			hockey_teams.Add(hockey_team);
		}
	}

	ForwardList<HockeyTeam> &GetHockeyTeams() noexcept
	{
		return hockey_teams;
	}
};

/*
* Entry point in program
*/
int main()
{
	/* Name of the hockey team data file */
	std::string filename = "HockeyTeams.ht";

	/* The program runs indefinitely unless the user enters the appropriate command. */
	while (true)
	{
		std::cout <<
			"0: End the program\n"
			"1: Read data from file\n"
			"2: Add data\n"
			"3: Delete data\n"
			"4: Find a team by last name\n"
			"5: Find the most effective team\n";

		int cmd;
		std::cout << "\n> ";
		std::cin >> cmd;

		switch (cmd)
		{
			/* End the program */
			case 0:
			{
				return 0;
			}

			/* Read data from file */
			case 1:
			{
				std::ifstream is{ filename };
				for (std::string line; std::getline(is, line);)
				{
					std::cout << line << '\n';
				}
				std::cout << '\n';

				break;
			}

			/* Add data */
			case 2:
			{
				HockeyTeams hockey_teams;
				hockey_teams.Deserialize(std::fstream{ filename });

				int number_of_attackers;
				std::cout << "Enter the number of attackers: ";
				std::cin >> number_of_attackers;

				ForwardList<std::string> names_of_attackers;

				std::cout << "Enter attacker's name:\n";
				for (auto i{ 0 }; i < number_of_attackers; ++i)
				{
					std::cout << std::to_string(i) + ": ";

					std::string attackers_name;
					std::cin >> attackers_name;

					names_of_attackers.Add(attackers_name);
				}

				HockeyTeam hockey_team{ .names_of_attackers = names_of_attackers };

				std::cout << "Enter team name: ";
				std::cin >> hockey_team.team_name;

				std::string number_of_goals_scored;
				std::cout << "Enter the number of goals scored: ";
				std::cin >> number_of_goals_scored;
				std::cout << '\n';

				hockey_team.number_of_goals_scored = std::stoi(number_of_goals_scored);

				hockey_teams.GetHockeyTeams().Add(hockey_team);
				hockey_teams.Serialize(std::fstream{ filename, std::ofstream::out | std::ofstream::trunc });
				break;
			}

			/* Delete data */
			case 3:
			{
				HockeyTeams hockey_teams;
				hockey_teams.Deserialize(std::fstream{ filename });

				hockey_teams.GetHockeyTeams().Del();

				hockey_teams.Serialize(std::fstream{ filename, std::ofstream::out | std::ofstream::trunc });
				break;
			}

			/* Find a team by last name */
			case 4:
			{
				HockeyTeams hockey_teams;
				hockey_teams.Deserialize(std::fstream{ filename });

				std::string last_name;
				std::cout << "Enter the last name of the player whose team you want to find: ";
				std::cin >> last_name;

				auto team = hockey_teams.SearchByLastName(last_name);

				/* If the team is not found */
				if (team == hockey_teams.GetHockeyTeams().end())
				{
					std::cout << "This team is not in the list\n\n";
					break;
				}

				std::cout << "Names of attackers: ";
				for (auto &&it : team->names_of_attackers)
				{
					std::cout << it << ' ';
				}
				std::cout << '\n';
				std::cout << "Team name: " + team->team_name + '\n';
				std::cout << "Number of goals scored: " + std::to_string(team->number_of_goals_scored) + "\n\n";
				break;
			}

			/* Find the most effective team */
			case 5:
			{
				HockeyTeams hockey_teams;
				hockey_teams.Deserialize(std::fstream{ filename });

				auto team = hockey_teams.MostScoringGame();

				/* If the team is not found */
				if (team == hockey_teams.GetHockeyTeams().end())
				{
					std::cout << "This team is not in the list\n\n";
					break;
				}

				std::cout << "Most Efficient Team:\n";

				std::cout << "Names of attackers: ";
				for (auto &&it : team->names_of_attackers)
				{
					std::cout << it << ' ';
				}
				std::cout << '\n';
				std::cout << "Team name: " + team->team_name + '\n';
				std::cout << "Number of goals scored: " + std::to_string(team->number_of_goals_scored) + "\n\n";

				break;
			}

			/* Incorrect input */
			default:
			{
				std::cout << "Unsupported command\n\n";
			}
		}
	}
}