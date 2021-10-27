/* Muistipeli
 *
 * Kuvaus:
 * Ohjelma toteuttaa muistipelin graafisella käyttöliittymällä. Pelissä on
 * käyttäjän valittavissa oleva määrä kortteja (2-36) ja pelaajia (1-4).
 * Peli käynnistetään play-painikkeella, jonka jälkeen vuorossa oleva pelaaja
 * arvaa kaksi kortti pelikentältä. Jos pelaaja sai parit, kortit poistetaan
 * pelilaudalta, pelaajan pistesaldoa kasvatetaan, ja hän saa uuden vuoron.
 * Jos pelaaja ei saanut pareja, kortit käännetään takaisin piiloon, ja vuoro
 * siirtyy seuraavalle pelaajalle.
 * Peli päättyy, kun kaikki parit on löydetty, ja pelilauta on tyhjä.
 * Lopuksi tulostetaan pelaajien löytämät parimäärät.
 *
 * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi

* */

#include "dialog.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog dialog;
    dialog.show();
    return a.exec();
}
