package catsatack.level;

import catsatack.personagens.Boss;
import catsatack.Game;
import javafx.scene.image.Image;
/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */

public class Level5 {
    private static Level5 level = null;
    private static final Image cenario = new Image("fundo5.png", 0, 600, true, true);

    private Level5() {

    }

    public static Level5 getInstance() {
        if (level == null) {
            level = new Level5();
        }
        return (level);
    }

    public void levelStart() {

        Game.getInstance().setFundo(cenario);
        // Adiciona inimigos
        if (Game.getInstance().getNPersonagens() == 1) {

            Game.getInstance().addChar(new Boss(200, 80));
            
        }
    }
}
