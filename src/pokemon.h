#ifndef PIKACHUGAME_POKEMON_H
#define PIKACHUGAME_POKEMON_H

#include <iostream>

#ifndef PIKACHUGAME_DRAWER_H
#include "drawer.h"
#endif

using namespace std;

#endif //PIKACHUGAME_POKEMON_H

struct Pokemon{
	string name;
	string desc;
	AnsiArt art;
};

static struct PokemonList{
	int size = 0;
	int current = -1;
	Pokemon* pokemons = nullptr;
	PokemonList(int s) {
		size = s;
		pokemons = new Pokemon[size];
	}
} pokemonList(3);

bool addPokemon(const string &name, const string &desc, AnsiArt art) {
	if (pokemonList.current >= pokemonList.size) return false;
	pokemonList.pokemons[pokemonList.current + 1].name = name;
	pokemonList.pokemons[pokemonList.current + 1].desc = desc;
	pokemonList.current++;
	return true;
}

// Pokemon 1: Pikachu
// Pokemon 2: Skitty
// Pokemon 3: Growlithe

bool loadPokemon() {
	addPokemon("Pikachu", "Pikachu, the sparky dynamo! This electric mouse crackles with energy, its fur standing on end. Don't let its cuteness fool you - a jolt from Pikachu can light up your whole journey, literally!", AnsiArt{{}});
	addPokemon("Skitty", "Skitty, the playful pounce! This adorable pink feline is full of pep and loves to chase its tail (and maybe steal your hat!). Don't underestimate its speed though - Skitty's playful swats can pack a surprising punch.", AnsiArt{{}});
	addPokemon("Growlithe", "Growlithe, the loyal pup! This fiery canine is all about courage and friendship. With a flame burning in its tail, Growlithe will fiercely protect you on your adventure. It may be a bit rough around the edges, but its loyalty knows no bounds.", AnsiArt{{}});
	return true;
}

void uninitPokemon() {
	delete[] pokemonList.pokemons;
	pokemonList.pokemons = nullptr;
}
