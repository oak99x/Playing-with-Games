package catsatack.personagens;
import javafx.scene.canvas.GraphicsContext;

/**
 * Represents the basic game character
 * @author Bernardo Copstein and Rafael Copstein
 */

public interface Character {
    int getX();
    int getY();
    int getAltura();
    int getLargura();

    void testaColisao(Character c);
    boolean jaColidiu();
    void setColidiu();

    void start();
    boolean isActive();
    void Update(long deltaTime);
    void Draw(GraphicsContext graphicsContext);
}
