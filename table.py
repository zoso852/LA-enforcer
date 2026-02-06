def generate_table(num_elements, max_value, max_index):
    table = []
    for i in range(num_elements):
        if i <= max_index:
            # montee : 1 -> max_value
            value = int(1 + (max_value - 1) * i / float(max_index))
        else:
            # descente : max_value -> 1
            value = int(1 + (max_value - 1) * (num_elements - 1 - i) / float(num_elements - 1 - max_index))
        table.append(value)
    return table

def main():
    # demandes a l'utilisateur
    num_elements = int(raw_input("Nombre d'elements : "))  # ou input() si Python 3
    max_value = int(raw_input("Valeur maximale : "))
    max_index = int(raw_input("Indice du maximum : "))

    # generation du tableau
    table = generate_table(num_elements, max_value, max_index)

    # ecriture dans un fichier
    filename = "acc_table.txt"
    with open(filename, "w") as f:
        f.write(",".join(str(v) for v in table))

    print("Tableau genere avec {} elements et ecrit dans '{}'.".format(num_elements, filename))

if __name__ == "__main__":
    main()
