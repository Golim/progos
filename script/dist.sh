#!/bin/bash
ID_GRUPPO='LABSO1-2017_2018--186009_187679_188255'

rm -rf ./$ID_GRUPPO
rm -rf ./$ID_GRUPPO.zip

mkdir ./$ID_GRUPPO

cp -u    Makefile          ./$ID_GRUPPO
cp -ru   src               ./$ID_GRUPPO
cp -u     descrizione.pdf  ./$ID_GRUPPO

zip -r $ID_GRUPPO.zip $ID_GRUPPO/