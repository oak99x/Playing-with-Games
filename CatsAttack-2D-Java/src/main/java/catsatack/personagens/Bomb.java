package catsatack.personagens;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Paint;

/**
 * Represents a shot that crosses the screen from bottom to up and then dismiss
 * @author Bernardo Copstein and Rafael Copstein
 */
/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */
public class Bomb extends BasicElement{
    public Bomb(int px,int py){
        super(px,py);
    }

    @Override
    public void start(){
        setDirV(1);
        setSpeed(1);
    }

    @Override
    public void testaColisao(Character outro){
        // para nao dar o bug com a colisão, onde o invasor atirava e bomba logo era desativada.
        if (outro instanceof Invasores1 || outro instanceof Invasores2 ||
            outro instanceof Invasores3 || outro instanceof Invasores4 ||
            outro instanceof Bomb || outro instanceof Boss){
            return;
        }else{
            super.testaColisao(outro);
        }
    }

    @Override
    public void Update(long deltaTime){
        if (jaColidiu()){
            deactivate();
        }else{
            setPosY(getY() + getDirV() * getSpeed());
            // Se chegou na parte inferior da tela ...
            if (getY() >= getLMaxV()){
                // Desaparece
                deactivate();
            }
        }
    }

    @Override
    public int getAltura(){
        return 15;
    }

    @Override
    public int getLargura(){
        return 15;
    }

    public void Draw(GraphicsContext graphicsContext){
        graphicsContext.setFill(Paint.valueOf("#FF0000"));
        graphicsContext.fillOval(getX(), getY(), 15, 15);
    }
}