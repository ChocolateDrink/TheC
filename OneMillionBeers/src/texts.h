#pragma once

#include <vector>
#include <random>

using namespace std;

struct MBText {
	LPCTSTR text;
	LPCTSTR title;
};

namespace texts {
	const vector<MBText> texts = {
		{TEXT("freakbob here"), TEXT("your phone is ringing")},
		{TEXT("yo"), TEXT("beer and car keys")},
		{TEXT("hey yo what up"), TEXT("rainbow spaghetti")},
		{TEXT("absolute chaos"), TEXT("gorilla dance party")},
		{TEXT("no time to explain"), TEXT("pink flamingo invasion")},
		{TEXT("hello darkness my old friend"), TEXT("toaster is sentient")},
		{TEXT("sponge memes for breakfast"), TEXT("don�t touch my banana")},
		{TEXT("weird flex but okay"), TEXT("robot chicken supremacy")},
		{TEXT("gonna yeet this whole thing"), TEXT("don�t question the pizza")},
		{TEXT("who needs sleep"), TEXT("giraffes are secretly spies")},
		{TEXT("bananas are the new currency"), TEXT("bring the waffles")},
		{TEXT("the void speaks"), TEXT("this is a fishbowl")},
		{TEXT("clocks are lying"), TEXT("time is a social construct")},
		{TEXT("eat the pickle, become the pickle"), TEXT("robot overlords will arrive")},
		{TEXT("I am potato now"), TEXT("get the cheese out of here")},
		{TEXT("emotional support cactus"), TEXT("sprinkle the chaos dust")},
		{TEXT("pee-pee poopoo time"), TEXT("spaghetti tornado")},
		{TEXT("unicorns are real, fight me"), TEXT("scream into the void")},
		{TEXT("one does not simply stop the memes"), TEXT("floating pickles everywhere")},
		{TEXT("bring the moon cheese"), TEXT("let the cows talk")},
		{TEXT("there�s a rat in the walls"), TEXT("fart noise intensifies")},
		{TEXT("glitch in the matrix"), TEXT("is this real life or a dream?")},
		{TEXT("who's driving this train?"), TEXT("oh wait, it�s just a muffin")},
		{TEXT("the waffles are plotting"), TEXT("don�t trust the toast")},
		{TEXT("anarchy in the snack aisle"), TEXT("I just got ninja kicked by a potato")},
		{TEXT("hello? can anyone hear me?"), TEXT("it�s raining meatballs")},
		{TEXT("don�t question the pizza rolls"), TEXT("they know too much")},
		{TEXT("zap zap boom boom"), TEXT("I�m a sentient alarm clock")},
		{TEXT("the sun is a lie"), TEXT("beep beep boop")},
		{TEXT("beep bop it�s a robot party"), TEXT("eat the chocolate cake")},
		{TEXT("I live for spaghetti day"), TEXT("let�s talk about the ducks")},
		{TEXT("muffin man stole my car"), TEXT("dinosaur disco party")},
		{TEXT("brainwave overload"), TEXT("this is fine, totally fine")},
		{TEXT("will trade socks for memes"), TEXT("chicken nugget diplomacy")},
		{TEXT("meatball conspiracy"), TEXT("the bees are watching")},
		{TEXT("but did you try turning it off?"), TEXT("banana phone is calling")},
		{TEXT("zombie apocalypse or nah"), TEXT("send help, but also snacks")},
		{TEXT("turtle party in my brain"), TEXT("whale song at 3AM")}
	};

	MBText getRandom() {
		random_device rng;
		mt19937 gen(rng());
		uniform_int_distribution<size_t> dis(0, texts.size() - 1);

		return texts[dis(gen)];
	}

	int getRandomInt() {
		random_device rng;
		mt19937 gen(rng());
		uniform_int_distribution<> dis(20, 50);

		return dis(gen) * 60 * 1000;
	}
}
