//////////////////////////////////////////
//	C++ Templates
//	Naughty Panda @ 2021
//////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cassert>

//////////////////////////////////////////
//	5.1
//////////////////////////////////////////

template <typename T>
class Pair1 {

private:
	T _first;
	T _second;

public:
	Pair1(const T& first, const T& second) : _first(first), _second(second) {}

	T first() const { return _first; }
	T second() const { return _second; }
};

//////////////////////////////////////////
//	5.2
//////////////////////////////////////////

template <typename F, typename S>
class Pair {

protected:
	F _first;
	S _second;

public:
	Pair(const F& first, const S& second) : _first(first), _second(second) {}
	virtual ~Pair() {}

	F first() const; // { return _first; }
	S second() const; // { return _second; }
};

template <typename F, typename S>
F Pair<F, S>::first() const { return _first; }

template <typename F, typename S>
S Pair<F, S>::second() const { return _second; }

//////////////////////////////////////////
//	5.3
//////////////////////////////////////////

template <typename T>
class StringValuePair : public Pair<std::string, T> {

public:
	StringValuePair(const std::string& str, const T& second) : Pair<std::string, T>(str, second) {}
	~StringValuePair() override {}
};

//////////////////////////////////////////
//	5.4
//////////////////////////////////////////

enum class ECardSuit : uint8_t {

	Clubs = 1U, Diamonds, Hearts, Spades,
	Undefined = 0U
};

enum class ECardValue : uint8_t {

	Two = 2U, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
	Jack = 10U, Queen = 10U, King = 10U, Ace = 1U,
	Undefined = 0U
};

class Card {

protected:
	ECardSuit _suit{ ECardSuit::Undefined };
	ECardValue _value{ ECardValue::Undefined };
	bool _bVisible{ false };

public:
	Card(ECardSuit suit, ECardValue val) : _suit(suit), _value(val) {}

	ECardValue GetValue() const { return _value; }
	void Flip() { _bVisible = !_bVisible; }
};

class Hand {

protected:
	std::vector<Card*> _hand;

public:
	virtual ~Hand() {}

	void Add(Card* card);
	void Clear();
	uint8_t GetValue() const;
};

class GenericPlayer : public Hand {

private:
	std::string _name{ "noname" };

public:
	GenericPlayer(const char* name) : _name(name) {}
	~GenericPlayer() override {}

	virtual bool IsHitting() const = 0;
	bool IsBoosted() const { return GetValue() > 21U ? true : false; }
	void Bust() const { std::cout << _name << " busted!\n"; }
};

void Hand::Add(Card* card) {

	assert(card);

	_hand.push_back(card);
}

void Hand::Clear() {

	_hand.erase(_hand.begin(), _hand.end());
}

uint8_t Hand::GetValue() const {

	uint8_t result{ 0U };
	uint8_t AceCount{ 0U };

	for (auto card : _hand) {

		if (card->GetValue() == ECardValue::Ace)
			++AceCount;
		else
			result += static_cast<int>(card->GetValue());
	}

	// Here we check if one/first of our Aces can be 11
	// It can be 11 if our hand value <= 10 - (AceCount - 1)
	if (AceCount != 0U && result <= 21U - 11U - (AceCount - 1U)) {

		result += 11U;
		--AceCount;
	}

	// If Ace cant't be 11, it'll be 1
	// for each of Aces left in hand
	result += AceCount;

	return result;
}



int main() {

	//////////////////////////////////////////
	//	5.1
	//////////////////////////////////////////

	Pair1<int> p1(6, 9);
	std::cout << "Pair: " << p1.first() << ' ' << p1.second() << '\n';

	const Pair1<double> p2(3.4, 7.8);
	std::cout << "Pair: " << p2.first() << ' ' << p2.second() << '\n';

	//////////////////////////////////////////
	//	5.2
	//////////////////////////////////////////

	Pair<int, double> p3(6, 7.8);
	std::cout << "Pair: " << p3.first() << ' ' << p3.second() << '\n';

	Pair<double, int> p4(3.4, 5);
	std::cout << "Pair: " << p4.first() << ' ' << p4.second() << '\n';

	//////////////////////////////////////////
	//	5.3
	//////////////////////////////////////////

	StringValuePair<int> svp("Amazing", 7);
	std::cout << "Pair: " << svp.first() << ' ' << svp.second() << '\n';

	StringValuePair<float> svp2("Amazing", 5.3f);
	std::cout << "Pair: " << svp2.first() << ' ' << svp2.second() << '\n';

	Pair<std::string, std::string>* svp3 = new StringValuePair<std::string>("Amaizing", "string");
	std::cout << "Pair: " << svp3->first() << ' ' << svp3->second() << '\n';
	delete svp3;
	svp3 = nullptr;

	//////////////////////////////////////////
	//	5.4
	//////////////////////////////////////////

	Hand hand;
	Card crd1{ ECardSuit::Clubs, ECardValue::Ace };
	Card crd2{ ECardSuit::Diamonds, ECardValue::Three };
	Card crd3{ ECardSuit::Hearts, ECardValue::Eight };

	hand.Add(&crd1);
	hand.Add(&crd2);
	hand.Add(&crd3);
	hand.Add(&crd1);

	std::cout << "\nHand value: " << static_cast<int>(hand.GetValue());

	hand.Clear();

	return 0;
}
