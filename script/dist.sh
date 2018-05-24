#!/bin/bash
#TODO: metti numeri matricola
ID_GRUPPO='LABSO1-2017_2018--nmatricole'

rm -rf ./$ID_GRUPPO

mkdir ./$ID_GRUPPO

cp -u    Makefile          ./$ID_GRUPPO
cp -ru   src               ./$ID_GRUPPO
cp -u     descrizione.pdf  ./$ID_GRUPPO