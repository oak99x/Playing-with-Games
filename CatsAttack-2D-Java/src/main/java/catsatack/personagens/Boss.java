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
public class Boss extends BasicElement {
    private long RELOAD_TIME = 1000000000; // Time is in nanoseconds
    private long shot_timer = 0;
    private Image boss;
    private int vidaBoss = 20;
    public static final Image explosion_image = new Image("explo.png", 0, 90, true, true);
    private static final Image bossForte = new Image("boss1.png", 0, 100, true, true);
    private static final Image bossFraco = new Image("boss2.png", 0, 100, true, true);
    private boolean explosion = false;

    public Boss(int px, int py) {
        super(px, py);
        try {
            // Carrega a imagem ajustando a altura para 40 pixels
            // mantendo a proporção em ambas dimensões
            boss = bossForte;
        } catch (Exception e) {
            System.out.println(e.getMessage());
            System.exit(1);
        }
    }

    @Override
    public void start() {

        setDirH(1);
        setSpeed(5);// ---------
    }

    @Override
    public void Update(long deltaTime) {

        if (vidaBoss <= 10) {
            try {
                boss = bossFraco;
            } catch (Exception e) {
                System.out.println(e.getMessage());
                System.exit(1);
            }
        }

        if (jaColidiu()) {
            // diminui vidas até zera
            vidaBoss--;
            if (vidaBoss <= 0) {
                explosion = true;
                Game.getInstance().incPontos();
                Game.getInstance().setGameWin();
               // deactivate();
            }
            // faz o jaColidiu a voltar a ser false
            setColidiuVolta();

        } else {
            setPosX(getX() + getDirH() * getSpeed());
            if (shot_timer > 0) {
                shot_timer -= deltaTime;
            }
            // Se chegou no lado direito da tela ...
            if (getX() >= getLMaxH() || getX() < getLMinH()) {
                // Inverte a direção
                setDirH(getDirH() * -1);
                // Sorteia o passo de avanço [1,5]
                setSpeed(Params.getInstance().nextInt(3) + 7);
                // Se ainda não chegou perto do chão, desce
            }
        }

        this.bombOn();// gerar bomba
    }

    //metodo para os inimigos atirarem bomba
    public void bombOn() {
        int aux = new Random().nextInt(50);
        if (aux == 0) {
            if (shot_timer <= 0) {
                Game.getInstance().addChar(new Bomb(getX(), getY() + 5));
                shot_timer = RELOAD_TIME;
            }
        }

    }

    public void Draw(GraphicsContext graphicsContext) {
        graphicsContext.drawImage(boss, getX(), getY());

        if (explosion) {
            graphicsContext.drawImage(explosion_image, getX(), getY());
        }
    }

    @Override
    public void testaColisao(Character outro) {
        if (outro instanceof Boss || outro instanceof Bomb) {
            return;
        } else {
            super.testaColisao(outro);
        }
    }
}
