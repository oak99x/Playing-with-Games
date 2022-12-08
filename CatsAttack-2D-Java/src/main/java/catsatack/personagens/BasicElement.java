package catsatack.personagens;
import catsatack.Params;
import catsatack.Game;
import javafx.scene.canvas.GraphicsContext;

/**
 * Represents the basic game character
 * @author Bernardo Copstein and Rafael Copstein
 */
public abstract class BasicElement implements Character{

    int direction_horizontal = 0, direction_vertical = 0;
    int lminV = 0, lmaxV = Params.WINDOW_HEIGHT;
    int lminH = 0, lmaxH = Params.WINDOW_WIDTH;
    int largura = 32, altura = 32;
    boolean colidiu = false;
    boolean active = true;
    int posX, posY;
    int speed = 2;

    public BasicElement(int startX,int startY){
        posX = startX;
        posY = startY;
    }

    @Override
    public int getX(){
        return(posX);
    }

    @Override
    public int getY(){
        return(posY);
    }

    @Override
    public int getAltura(){
        return(altura);
    }

    @Override
    public int getLargura(){
        return(largura);
    }

    @Override
    public void testaColisao(Character outro){
        if (colidiu){
            return;
        }
        // Monta pontos
        int p1x = this.getX();
        int p1y = this.getY();
        int p2x = p1x+this.getLargura();
        int p2y = p1y+this.getAltura();

        int op1x = outro.getX();
        int op1y = outro.getY();
        int op2x = op1x+outro.getLargura();
        int op2y = op1y+outro.getAltura();

        // Verifica colisão
        if (p1x < op2x && p2x > op1x && p1y < op2y && p2y > op1y){
            colidiu = true;
        }
    }

    public int getDirH(){
        return(direction_horizontal);
    }

    public int getDirV(){
        return(direction_vertical);
    }

    public int getLMinH(){
        return(lminH);
    }

    public int getLMaxH(){
        return(lmaxH);
    }

    public int getLMinV(){
        return(lminV);
    }

    public int getLMaxV(){
        return(lmaxV);
    }

    public int getSpeed(){
        return(speed);
    }

    public void setPosX(int p){
        posX = p;
    }

    public void setPosY(int p){
        posY = p;
    }

    public void setLargAlt(int l,int a){
        largura = l;
        altura = a;
    }

    public void setDirH(int dirH){
        direction_horizontal = dirH;
    }

    public void setDirV(int dirV){
        direction_vertical = dirV;
    }

    public void setLimH(int min,int max){
        lminH = min;
        lmaxH = max;
    }

    public void setLimV(int min,int max){
        lminV = min;
        lmaxV = max;
    }

    public void setSpeed(int s){
        speed = s;
    }

    public void deactivate(){
        active = false;
        Game.getInstance().eliminate(this);
    }

    @Override
    public boolean jaColidiu(){
        return(colidiu);
    }

    @Override
    public void setColidiu(){
        colidiu = true;
    }
    //apos verificar e decrementar a vida do canhao volta a ser false, para testa uma nova colissao
    public void setColidiuVolta(){
        colidiu = false;
    }

    @Override
    public  boolean isActive(){
        return(active);
    }

    @Override
    public abstract void start();

    @Override
    public abstract void Update(long deltaTime);

    @Override
    public abstract void Draw(GraphicsContext graphicsContext);
}
