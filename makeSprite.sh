#!/bin/bash


if [-z "$1"]
    then
    echo "Please give a name to the output image"
fi

url="https://deckofcardsapi.com/static/img"

families=("C" "D" "H" "S")
values=("2" "3" "4" "5" "6" "7" "8" "9" "0" "J" "Q" "K" "A")

row=()
rows=()
for family in "${families[@]}"; do
  for value in "${values[@]}"; do
        curl -O $url/$value$family.png 
        if [ ! -z "$2" ]; then
            convert $value$family.png -resize $2% $value$family.png 
        fi
        row+=("$value$family.png") 
  done
  convert +append "${row[@]}" row$family.png
  rows+=(row$family.png)
  rm "${row[@]}"
  row=()
done
last=("X1.png" "X2.png" "back.png")
row=()
for l in "${last[@]}"; do
    curl -O $url/$l
    if [ ! -z "$2" ]; then
        convert $l -resize $2% $l 
    fi
    row+=("$l")
done
convert +append "${last[@]}" rowLast.png
rm "${last[@]}"
rows+=("rowLast.png")
convert -append "${rows[@]}" sprite.png
rm "${rows[@]}"
mv sprite.png assets/$1.png