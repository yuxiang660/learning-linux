#%%

# The probability of same birth day among N persons
import matplotlib.pyplot as plt
import math

NUM_DAYS_PER_YEAR = 365

def get_at_least_two_same_birth_appr_probs(num):
    return 1 - math.exp(-(num - 1) * num / (NUM_DAYS_PER_YEAR * 2))

num_of_persons = range(1, 50)
no_same_birth_prob = 1

# one point: (number of persons, probability)
at_least_two_same_birth_probs = []
for n in num_of_persons:
    no_same_birth_prob = ((NUM_DAYS_PER_YEAR - (n - 1)) / NUM_DAYS_PER_YEAR) * no_same_birth_prob
    at_least_two_same_birth_probs.append(1 - no_same_birth_prob)

plt.plot(num_of_persons, at_least_two_same_birth_probs, 'o', label='birth_prob_real')
plt.plot(num_of_persons,
        [get_at_least_two_same_birth_appr_probs(num) for num in num_of_persons],
        'o', label='birth_prob_appr')

plt.xlabel('Number of Persons')
plt.ylabel('Probability')
plt.title("At Least Two Same Birth Probability Plot")
plt.legend()

plt.show()
