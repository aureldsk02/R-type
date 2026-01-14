#!/bin/bash
echo -e "\nCe programme permet de commit nos travaux.\n"

echo -e "On commence par le formatage\nFormatage en cours...\n"
count=0
while IFS= read -r -d '' file; do
    count=$((count + 1))
    echo "Formatage de : $file"
    
    if ! clang-format -i "$file"; then
        echo "ERREUR sur : $file"
        exit 1
    fi
done < <(find engine/ rtype_client/ rtype_server/ utility_classes \( -name "*.cpp" -o -name "*.hpp"\) -print0)

if [ $count -eq 0 ]; then
    echo "Aucun fichier trouvé"
else
    echo "Formatage réussi sur $count fichier(s)"
fi

./compiling.sh

if [$? -ne 0 ]; then
    echo "Le programma a échoué. Vous n'avez pas le droit de commit"
    exit 1
fi

echo "Listez les fichiers à ajouter."
read ELEMENTS

if git add $ELEMENTS; then
    echo "Fichiers ajoutés avec succès."
else
    echo "Erreur lors de l'ajout des fichiers."
    exit 1
fi

echo -e "Select a number to related to what you're going to push\n1.Add\n2.Fix\n3.Refactor\n4.Delete\n5.Doc\n"

while true; do
    read -p "Make your choice (1-5) : " choice
    if [[ $choice =~ ^[1-5]$ ]]; then
        break
    else
        echo "Enter a valid number between 1 and 5"
    fi
done

echo "Redigez le commit"
read COMMITS

case $choice in 
    1) prefix="add:" ;;
    2) prefix="fix:" ;;
    3) prefix="refactor:" ;;
    4) prefix="delete:" ;;
    5) prefix="doc:" ;;
    *) echo "Choix invalide"; exit 1 ;;
esac

if git commit -m "$prefix $COMMITS"; then
    echo "Commit créé avec succès. ✅ "
else
    echo "Erreur lors de la création du commit. ❌"
    exit 1
fi

if git push; then
    echo "Modifications poussées avec succès."
else
    echo "Erreur lors du push vers le dépôt distant."
    exit 1
fi