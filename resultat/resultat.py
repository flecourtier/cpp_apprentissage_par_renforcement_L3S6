import matplotlib.pyplot as plt
import re

with open("sortie") as f:
    data=f.readlines()

resultats_regex = re.findall(r"(\[...\]) : (.*) %\n", "".join(data))
mu_opt = [ item[0] for item in resultats_regex ]
pourcentage = [ float(item[1]) for item in resultats_regex ]
print(mu_opt)
print(pourcentage)

plt.bar(range(1, 10), pourcentage, tick_label = mu_opt)
plt.xlabel("Mu_opt")
plt.ylabel("Fréquence (%)")
plt.title("Pourcentage des mu_opt obtenus après aprentissage")
plt.savefig("graphique_resultats.png")
plt.show()