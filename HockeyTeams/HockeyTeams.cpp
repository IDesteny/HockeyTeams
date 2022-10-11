#include <iostream>

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
	void Add(const T &dt)
	{
		head = new Node{ dt, head };
	}

	/* Linked list removal method */
	void Del()
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
	ForwardListIterator begin() const
	{
		return head;
	}

	/* Necessary for: range based for loop */
	ForwardListIterator end() const
	{
		return nullptr;
	}

	/*
	* Destructor.
	* Required to clear the contents of a singly linked list.
	* Called when an object goes out of scope
	*/
	~ForwardList()
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
	HockeyTeam &MostScoringGame()
	{
		auto max_value{ hockey_teams.begin() };

		for (auto hockey_team{ max_value }; hockey_team != hockey_teams.end(); ++hockey_team)
		{
			if (hockey_team->number_of_goals_scored > max_value->number_of_goals_scored)
			{
				max_value = hockey_team;
			}
		}

		return *max_value;
	}
};

int main()
{
	ForwardList<std::string> names_of_attackers_1;
	names_of_attackers_1.Add("Williams");
	names_of_attackers_1.Add("Peters");

	ForwardList<std::string> names_of_attackers_2;
	names_of_attackers_2.Add("Gibson");
	names_of_attackers_2.Add("Martin");

	ForwardList<HockeyTeam> hockey_teams;
	hockey_teams.Add({ names_of_attackers_1, "Anaheim Ducks", 3 });
	hockey_teams.Add({ names_of_attackers_2, "Arizona Coyotes", 5 });

	HockeyTeams hockey_teams_handler(hockey_teams);

	auto most_scoring_game = hockey_teams_handler.MostScoringGame();
	auto team_martin = hockey_teams_handler.SearchByLastName("Martin");
	auto team_williams = hockey_teams_handler.SearchByLastName("Williams");

	std::cout << "Team with the best stats: " << most_scoring_game.team_name << '\n';
	std::cout << "Team Martin: " << team_martin->team_name << '\n';
	std::cout << "Team Williams: " << team_williams->team_name << '\n';
}