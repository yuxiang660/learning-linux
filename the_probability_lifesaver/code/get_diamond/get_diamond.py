import random

def mimic_get_diamond_game(num_of_tries):
    awin = 0
    bwin = 1
    cwin = 2

    for i in range(num_of_tries):
        while True:
            acard = random.randint(1, 52)
            if acard <= 13:
                awin += 1
                break
            bcard = random.randint(1, 52)
            if bcard <= 13:
                bwin += 1
                break
            ccard = random.randint(1, 52)
            if ccard <= 13:
                cwin += 1
                break

    return [awin, bwin, cwin]

if __name__ == '__main__':
    NUM_OF_TRIES = int(1e6)
    [awin, bwin, cwin] = mimic_get_diamond_game(NUM_OF_TRIES)
    print(f"Here are the observed probabilities from {NUM_OF_TRIES} games.")
    print(f"Percent Alice won (approx): {100.0 * awin / NUM_OF_TRIES} %")
    print(f"Percent Bob won (approx): {100.0 * bwin / NUM_OF_TRIES} %")
    print(f"Percent Charlie won (approx): {100.0 * cwin / NUM_OF_TRIES} %")
