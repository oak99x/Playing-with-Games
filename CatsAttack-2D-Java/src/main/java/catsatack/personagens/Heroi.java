package catsatack.personagens;
import catsatack.KeyboardCtrl;
import catsatack.Params;
import catsatack.Game;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.input.KeyCode;

/**
 * Represents the game Gun
 * @author Bernardo Copstein, Rafael Copstein
 */
/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */
public class Heroi extends BasicElement implements KeyboardCtrl{
    private int RELOAD_TIME = 100000000; // Time is in nanoseconds
    private int shot_timer = 0;
    private Image heroi;
    public static final Image  explosion_image = new Image( "explo.png",0,90,true,true );
    private boolean explosion =false;

    public Heroi(int px,int py, Image image){
        super(px,py);
        try{
            // Carrega a imagem ajustando a altura para 40 pixels
            // mantendo a proporção em ambas dimensões
                heroi = image;
        }catch(Exception e){
            System.out.println(e.getMessage());
            System.exit(1);
        }
    }

    @Override
    public void start() {
        setLimH(20,Params.WINDOW_WIDTH-20);
       setLimV(Params.WINDOW_HEIGHT-100,Params.WINDOW_HEIGHT);
    }

    @Override
    public void Update(long deltaTime) {

        if (jaColidiu()){
            //diminui vidas até zera 
            Game.getInstance().decVidas();
            if(Game.getInstance().getVidas()<=0){
                explosion=true;
                Game.getInstance().setGameOver();
            }
        }
        //a cada inimigo que chega no solo o heroi perde uma vida,
        //o que pode resultar em game over também
        if(Game.getInstance().getVidas()<=0){
            explosion=true;
            Game.getInstance().setGameOver();
        }
        //faz o jaColidiu a voltar a ser false
        setColidiuVolta();
        setPosX(getX() + getDirH() * getSpeed());
        if (shot_timer > 0) shot_timer -= deltaTime;
    }


    @Override
    public void OnInput(KeyCode keyCode, boolean isPressed) {
        if (keyCode == KeyCode.LEFT){
            int dh = isPressed ? -1 : 0; 
            setDirH(dh);
        }
        if (keyCode == KeyCode.RIGHT){
            int dh = isPressed ? 1 : 0;
            setDirH(dh);
        }
        if (keyCode == KeyCode.SPACE){
            if (shot_timer <= 0) {
                Game.getInstance().addChar(new Shot(getX()+16,getY()-32));
                shot_timer = RELOAD_TIME;
            }
        }
    }

    @Override
    public int getAltura(){
        return 100;
    }

    @Override
    public int getLargura(){
        return 50;
    }

    @Override
    public void Draw(GraphicsContext graphicsContext) {
       graphicsContext.drawImage(heroi, getX(),getY());
       if(explosion){
        graphicsContext.drawImage(explosion_image, getX(),getY());   
        }

    }
}
