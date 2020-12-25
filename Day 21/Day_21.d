module Day_21;

import std.stdio;
import std.string;
import std.algorithm;

class IngredientList {
public:
    string[] ingredients;
    string[] sus_ingredients;
    string[] allergens;
    this(string contents) {
        auto strings = contents.strip("\r").strip(")").split(" (contains ");
        foreach (ingredient; strings[0].split(" ")) {
            ingredients ~= ingredient;
        }
        foreach (allergen; strings[1].split(", ")) {
            allergens ~= allergen;
        }
        // writeln(ingredients, allergens);
    }
}

class IngredientInfo {
public:
    string name;
    string[] ruled_out;
    string[] sus_allergens;
    int appearances;
    this(string _name) {
        appearances = 1;
        name = _name;
    }

    void populate_sus_allergens(string[] allergens) {
        foreach (allergen; allergens) {
            if (!ruled_out.canFind(allergen)) {
                sus_allergens ~= allergen;
            }
        }
    }

    void rule_more_out(string ruled_out) {
        string[] new_sus_allergens;
        foreach (allergen; sus_allergens) {
            if (ruled_out != allergen)
                new_sus_allergens ~= allergen;
        }
        sus_allergens = new_sus_allergens;
    }
}

auto parse_input(string filename)
{
    auto file = File(filename);
    char[] buffer;
    string raw_contents;
    while (file.readln(buffer)) {
        raw_contents ~= buffer;
    }
    auto lines = raw_contents.strip().split("\n");

    IngredientList[] food;
    foreach (line; lines) {
        food ~= new IngredientList(line);
    }

    return food;
}

auto clear_ingredients(IngredientList[] foods) {
    IngredientInfo[string] ingredients;
    string[] allergens;
    string[] cleared_ingredients;
    int total_clear;
    int unidentified_ingredients = 0;
    // First pass: just find out how many ingredients and allergens we have!
    foreach (food; foods) {
        foreach (ingred; food.ingredients) {
            // I won't be updating this to use it since it's a single-purpose script, but
            // while looking for something else I came across .require in the docs
            // for "add thing if thing isn't there" - though this particular instance might not be able
            // to use it thanks to the else handler... but I don't this is the only place I've used this
            auto ptr = (ingred in ingredients);
            if (ptr is null) {
                ingredients[ingred] = new IngredientInfo(ingred);
            }
            else ptr.appearances++;
        }
        foreach (allergen; food.allergens) {
            if (!allergens.canFind(allergen)) {
                allergens ~= allergen;
                unidentified_ingredients++;
            }
        }
    }

    // Second pass: check what foods can't be an allergen!
    // Approach: If an allergen is listed on an ingredient list that does not contain the ingredient we're looking at,
    // we know those allergens aren't present thanks to the guarantees in the problem statement.
    // If we rule out every allergen, then it's clear!
    foreach (ingredient; ingredients) {
        foreach (food; foods) {
            if (!food.ingredients.canFind(ingredient.name)) {
                foreach (allergen; food.allergens) {
                    // Should really look into seeing if D has a set, since that's basically what we're doing
                    if (!ingredient.ruled_out.canFind(allergen))
                        ingredient.ruled_out ~= allergen;
                }
            }
        }
        if (ingredient.ruled_out.length == allergens.length) {
            cleared_ingredients ~= ingredient.name;
            total_clear += ingredient.appearances;
            writeln("Cleared ingredient: ", ingredient.name, "\t(occurs ", ingredient.appearances, " time(s))");
        }
    }
    writeln("Safe ingredients occur ", total_clear, " times.");

    // Finally, use what we learned here to clean up a bit.
    foreach (food; foods) {
        foreach (ingredient; food.ingredients) {
            if (!cleared_ingredients.canFind(ingredient)) food.sus_ingredients ~= ingredient;
        }
    }
    
    // Just kidding! We're going to keep going from Part 2, but that's because we'd have to return
    // quite a lot from this function to put Part 2 in another one, and I think the resulting
    // function hot-potato would in this specific case be less readable than one monster function.
    
    // Part 2 init;
    foreach (ingredient; ingredients) {
        ingredient.populate_sus_allergens(allergens);
    }

    // To help me remember where I left off: This code infinite loops here.
    // Best guess is that the sus_allergens list is a bit sus... should investigate
    string[string] allergen_ingredient_pairs;
    while (unidentified_ingredients > 0) {
        foreach (ingredient; ingredients) {
            if (ingredient.sus_allergens.length == 1) {
                auto allergen_id = ingredient.sus_allergens[0];
                allergen_ingredient_pairs[allergen_id] = ingredient.name;
                unidentified_ingredients--;
                foreach (ingred; ingredients) {
                    ingred.rule_more_out(allergen_id);
                }
                break;
            }
        }
    }
    writeln(allergen_ingredient_pairs);

    // Finally, assemble the output it wants. We'll definitely want to write this to a file, because
    // copy/pasting from the Windows CLI isn't fun.
    string output;
    auto keys = allergen_ingredient_pairs.keys;
    keys.sort();
    foreach (i, key; keys) {
        if (i != 0) output ~= ",";
        output ~= allergen_ingredient_pairs[key];
    }
    auto file = File("output.txt", "w");
    file.writeln(output);
    writeln(output);
}

void main()
{
    auto foods = parse_input("input.txt");
    clear_ingredients(foods);
}
