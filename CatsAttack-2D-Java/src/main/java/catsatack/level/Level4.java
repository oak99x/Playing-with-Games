package catsatack.level;

import catsatack.personagens.Invasores4;
import catsatack.Game;
import java.util.Random;
import javafx.scene.image.Image;
/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */

public class Level4 {
    private static Level4 level = null;
    private static final Image inimigo = new Image("cat4.png", 0, 80, true, true);
    private static final Image cenario = new Image("fundo4.png", 0, 600, true, true);

    private Level4() {

    }

    public static Level4 getInstance() {
        if (level == null) {
            level = new Level4();
        }
        return (level);
    }

    public void levelStart() {

        Game.getInstance().setFundo(cenario);
        // Adiciona inimigos
        if (Game.getInstance().getNPersonagens() == 1) {
            
            for (int i = 0; i < 3; i++) {
                int pos = new Random().nextInt(18);
                Game.getInstance().addChar(new Invasores4(100 + (pos * 20), 60, inimigo));
            }
        }
    }

    public void geraInimigos() {

        if (Game.getInstance().getNPersonagens() < 4) {
            int aux = new Random().nextInt(50);
            //Pode ou não gerar inimigos
            if (aux == 0) {
                int pos = new Random().nextInt(18);
                Game.getInstance().addChar(new Invasores4(80 + (pos * 20), 60, inimigo));
            }
        }
    }
}
