package catsatack;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.input.KeyCode;
import catsatack.level.Level1;
import catsatack.level.Level2;
import catsatack.level.Level3;
import catsatack.level.Level4;
import catsatack.level.Level5;
import catsatack.personagens.Character;
import catsatack.personagens.Heroi;

import java.util.List;
import java.util.Random;
import java.util.LinkedList;
import javafx.scene.image.Image;

/**
 * Handles the game lifecycle and behavior
 * 
 * @author Bernardo Copstein and Rafael Copstein
 */
/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */
public class Game {
    private static Game game = null;
    private Heroi heroi;
    private List<Character> activeChars;
    private boolean gameOver;
    private boolean gameWin;
    private int pontos;
    private int vidas = 3;
    private Image cenario;
    private int level = 0;
    private int minPontos = 0;

    private static final Image HEROILIST[] = { new Image("girl.png", 0, 80, true, true) };

    private Game() {
        gameOver = false;
        gameWin = false;
        pontos = 0;
    }

    public void setGameOver() {
        gameOver = true;
    }

    public boolean isGameOver() {
        return gameOver;
    }

    public void setGameWin() {
        gameWin = true;
    }

    public boolean isGameWin() {
        return gameWin;
    }

    public int getPontos() {
        return pontos;
    }

    public void incPontos() {
        pontos++;
    }

    public int getVidas() {
        return vidas;
    }

    public void decVidas() {
        vidas--;
    }

    public void setFundo(Image c) {
        cenario = c;
    }

    public void getRestart() {
        game = null;
    }

    public static Game getInstance() {
        if (game == null) {
            game = new Game();
        }
        return (game);
    }

    public void addChar(Character c) {
        activeChars.add(c);
        c.start();
    }

    public int getNPersonagens() {
        return activeChars.size();
    }

    public void eliminate(Character c) {
        activeChars.remove(c);
    }

    public void Start() {
        // Repositório de personagens
        activeChars = new LinkedList<>();

        // Adiciona o heroi
        heroi = new Heroi(350, 500, HEROILIST[0]);
        activeChars.add(heroi);

        level();
    }

    public void Update(long currentTime, long deltaTime) {

        if (gameOver) {
            return;
        }

       level();

        for (int i = 0; i < activeChars.size(); i++) {
            Character este = activeChars.get(i);
            este.Update(deltaTime);
            for (int j = 0; j < activeChars.size(); j++) {
                Character outro = activeChars.get(j);
                if (este != outro) {
                    este.testaColisao(outro);
                }
            }
        }

    }

    public void OnInput(KeyCode keyCode, boolean isPressed) {
        heroi.OnInput(keyCode, isPressed);
    }

    public void Draw(GraphicsContext graphicsContext) {
        graphicsContext.drawImage(cenario, 0, 0);
        for (Character c : activeChars) {
            c.Draw(graphicsContext);
        }
    }

    public void level() {
        
        if (activeChars.size() == 1 && getPontos()>= minPontos){
            level++;
            minPontos+=4;
        }

        switch (level) {
            case 1:
                    Level1.getInstance().levelStart();
                    Level1.getInstance().geraInimigos();
                break;
            case 2:
                    Level2.getInstance().levelStart();
                    Level2.getInstance().geraInimigos();
                break;
            case 3:
                    Level3.getInstance().levelStart();
                    Level3.getInstance().geraInimigos();
                break;
            case 4:
                    Level4.getInstance().levelStart();
                    Level4.getInstance().geraInimigos();
                break;
            case 5:
                    Level5.getInstance().levelStart();
                break;
            default: 
                    this.setGameWin();
                break;
        }

    }
}
