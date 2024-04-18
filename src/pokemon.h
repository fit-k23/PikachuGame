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
	pokemonList.pokemons[pokemonList.current + 1].art = art;
	pokemonList.current++;
	return true;
}

// Pokemon 1: Pikachu
// Pokemon 2: Skitty
// Pokemon 3: Growlithe

bool loadPokemon() {
	AnsiArt pikachuAnsi;
	pikachuAnsi.addFrame(getFileContent(string(TEXT_RELATIVE_PATH) + "pokemon_pikachu.txt"));
	addPokemon("Pikachu", "Pikachu, the sparky dynamo! This electric mouse crackles with energy, its fur standing on end.\nDon't let its cuteness fool you - a jolt from Pikachu can light up your whole journey, literally!", pikachuAnsi);

	AnsiArt skittyAnsi;
	skittyAnsi.addFrame(getFileContent(string(TEXT_RELATIVE_PATH) + "pokemon_skitty.txt"));
	addPokemon("Skitty", "Skitty, the playful pounce! This adorable pink feline is full of pep and loves to chase its tail (and maybe steal your hat!).\nDon't underestimate its speed though - Skitty's playful swats can pack a surprising punch.", skittyAnsi);

	AnsiArt growlitheAnsi;
	growlitheAnsi.addFrame(getFileContent(string(TEXT_RELATIVE_PATH) + "pokemon_growlithe.txt"));
	addPokemon("Growlithe", "Growlithe, the loyal pup! This fiery canine is all about courage and friendship.\nWith a flame burning in its tail, Growlithe will fiercely protect you on your adventure.\nIt may be a bit rough around the edges, but its loyalty knows no bounds.", growlitheAnsi);
	return true;
}

void uninitPokemon() {
	delete[] pokemonList.pokemons;
	pokemonList.pokemons = nullptr;
}
