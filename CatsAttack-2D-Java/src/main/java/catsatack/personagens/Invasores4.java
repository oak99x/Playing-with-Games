package catsatack.personagens;

import java.util.Random;

import catsatack.Game;
import catsatack.Params;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;

/**
 * Represents a simple ball that crosses the screen over and over again
 * 
 * @author Bernardo Copstein and Rafael Copstein
 */
/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */
public class Invasores4 extends BasicElement {
    private Image invasor;
    private static final Image explosion_image = new Image("explo.png", 0, 90, true, true);
    private boolean explosion = false;
    

    public Invasores4(int px, int py, Image image) {
        super(px, py);
        try {
            // Carrega a imagem ajustando a altura para 40 pixels
            // mantendo a proporção em ambas dimensões
            invasor = image;
        } catch (Exception e) {
            System.out.println(e.getMessage());
            System.exit(1);
        }
    }

    @Override
    public void start() {

        setDirV(1);
        setSpeed(1);
    }

    @Override
    public void Update(long deltaTime) {
        //verifica se chegou ao solo
        if (getY() >= Params.WINDOW_HEIGHT-100) {
            explosion=true;
           Game.getInstance().setGameOver();
          // Game.getInstance().decVidas();
          // deactivate();
        }
        
        if (jaColidiu()) {
            Game.getInstance().incPontos();
            deactivate();
               
        } else {
            setPosY(getY() + 2);
        }
        
    }

    public void Draw(GraphicsContext graphicsContext) {
        graphicsContext.drawImage(invasor, getX(), getY());
        if(explosion){
            graphicsContext.drawImage(explosion_image, getX(),getY());   
            }
    }

    @Override
    public void testaColisao(Character outro) {

        if (outro instanceof Invasores4 || outro instanceof Bomb) {
            return;
        } else {
            super.testaColisao(outro);
        }
    }
}
