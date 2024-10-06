import numpy as np
import matplotlib.pyplot as plt

# Partie 1 : Visualisation des nombres uniformes
def visualiser_nombres_uniformes():
    a, b = -98, 57.7
    uniformes = np.random.uniform(a, b, 10000)  # Générer 10 000 nombres
    plt.hist(uniformes, bins=50, color='blue', alpha=0.7, edgecolor='black')
    plt.title('Distribution Uniforme')
    plt.xlabel('Valeurs')
    plt.ylabel('Fréquence')
    plt.show()

# Partie 2 : Visualisation de la loi exponentielle négative
def visualiser_nombres_exponentiels():
    moyenne = 10
    exponentiels = np.random.exponential(scale=moyenne, size=10000)
    plt.hist(exponentiels, bins=50, color='orange', alpha=0.7, edgecolor='black')
    plt.title('Distribution Exponentielle Négative')
    plt.xlabel('Valeurs')
    plt.ylabel('Fréquence')
    plt.show()

# Partie 3 : Visualisation de la distribution discrète
def visualiser_distribution_discrete():
    proportions = [0.35, 0.45, 0.20]
    echantillons = np.random.choice(['Classe A', 'Classe B', 'Classe C'], size=10000, p=proportions)
    unique, counts = np.unique(echantillons, return_counts=True)
    plt.bar(unique, counts, color=['red', 'green', 'blue'])
    plt.title('Distribution Discrète (Classes A, B, C)')
    plt.xlabel('Classes')
    plt.ylabel('Fréquence')
    plt.show()

# Partie 4 : Visualisation de la loi normale par méthode de rejet
def f_normal(x):
    return (1.0 / np.sqrt(2 * np.pi)) * np.exp(-0.5 * x**2)

def visualiser_rejection_method():
    minX, maxX = -5, 5
    maxY = f_normal(0)
    samples = []
    for _ in range(10000):
        while True:
            Na1 = np.random.uniform()
            Na2 = np.random.uniform()
            x = minX + Na1 * (maxX - minX)
            y = maxY * Na3
            if y <= f_normal(x):
                samples.append(x)
                break
    plt.hist(samples, bins=50, color='purple', alpha=0.7, edgecolor='black')
    plt.title('Distribution Normale avec la Méthode de Rejet')
    plt.xlabel('Valeurs')
    plt.ylabel('Fréquence')
    plt.show()

# Partie 5 : Visualisation de la méthode de Box-Muller
def visualiser_box_muller():
    samples = []
    for _ in range(5000):  # 5000 paires = 10 000 nombres
        Rn1 = np.random.uniform()
        Rn2 = np.random.uniform()
        x1 = np.sqrt(-2.0 * np.log(Rn1)) * np.cos(2.0 * np.pi * Rn2)
        x2 = np.sqrt(-2.0 * np.log(Rn1)) * np.sin(2.0 * np.pi * Rn2)
        samples.append(x1)
        samples.append(x2)
    plt.hist(samples, bins=50, color='cyan', alpha=0.7, edgecolor='black')
    plt.title('Distribution Normale avec la Méthode de Box-Muller')
    plt.xlabel('Valeurs')
    plt.ylabel('Fréquence')
    plt.show()

# Menu pour sélectionner la partie à visualiser
def menu():
    print("Choisissez une partie à visualiser :")
    print("1 - Distribution Uniforme")
    print("2 - Distribution Exponentielle Négative")
    print("3 - Distribution Discrète")
    print("4 - Méthode de Rejet (Normale)")
    print("5 - Méthode de Box-Muller (Normale)")
    choix = input("Votre choix : ")

    if choix == '1':
        visualiser_nombres_uniformes()
    elif choix == '2':
        visualiser_nombres_exponentiels()
    elif choix == '3':
        visualiser_distribution_discrete()
    elif choix == '4':
        visualiser_rejection_method()
    elif choix == '5':
        visualiser_box_muller()
    else:
        print("Choix invalide. Veuillez réessayer.")

if __name__ == "__main__":
    menu()
