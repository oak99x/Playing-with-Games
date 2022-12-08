package catsatack.level;

import catsatack.personagens.Invasores3;
import catsatack.Game;
import java.util.Random;
import javafx.scene.image.Image;

/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */

public class Level3 {
    private static Level3 level = null;

    private static final Image inimigo1 = new Image("cat3.png", 0, 40, true, true);
    private static final Image cenario = new Image("fundo3.png", 0, 600, true, true);

    private Level3() {

    }

    public static Level3 getInstance() {
        if (level == null) {
            level = new Level3();
        }
        return (level);
    }

    public void levelStart() {

        Game.getInstance().setFundo(cenario);
        // Adiciona inimigos
        if (Game.getInstance().getNPersonagens() == 1) {
            for (int i = 0; i < 4; i++) {
                Game.getInstance().addChar(new Invasores3(80 + (i * 20), 60 + (i * 40), inimigo1));
            }
        }
    }

    public void geraInimigos() {

        if (Game.getInstance().getNPersonagens() < 4) {
            int aux = new Random().nextInt(200);
            if (aux == 0) {
                Game.getInstance().addChar(new Invasores3(200, 80, inimigo1));
            }
        }
    }
}
