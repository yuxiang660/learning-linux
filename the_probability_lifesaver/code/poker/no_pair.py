import random

def generate_a_shuffled_poker():
    poker = []
    for point in range(1, 14, 1):
        for suit in ['clubs', 'diamonds', 'hearts', 'spades']:
            poker.append((point, suit))
    random.shuffle(poker)
    return poker

def count_no_pair(num_of_tries):
    poker = generate_a_shuffled_poker()
    count = 0
    for i in range(num_of_tries):
        choices = random.sample(poker, 5)
        points = [p for p, _ in choices]
        if len(points) == len(set(points)):
            count += 1
    return count

if __name__ == "__main__":
    print("Probability of smallest 5 cards")
    NUM_OF_TRIES = 100000
    count = count_no_pair(NUM_OF_TRIES)
    print(f"Observed probability is {count/NUM_OF_TRIES*100}%")
