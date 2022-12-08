import catsatack.Game;
import catsatack.Params;
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;
import javafx.stage.Modality;
import javafx.stage.Stage;


/**
 * Handles window initialization and primary game setup
 * @author Bernardo Copstein, Rafael Copstein
 */
/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */

public class Main extends Application {

    private Group menuPane, gamePane, overPane, winPane;
    private Scene scene1, scene2, overScene, winScene;
    private Stage menuStage, gameStage, overStage, winStage;
    private Button btstart;
    private TextField userTextField;
    private String namePlayer;
    private CadastroPontuacao c;
    private boolean verificainicio =false;

    @Override
    public void start(Stage primaryStage){

        menuStage = primaryStage;

        menuPane = new Group();
        
        //titulo do game

        Text scenetitle1 = new Text("Space Invaders");
        scenetitle1.setId("spaceInvaders-text");
        scenetitle1.setFont(Font.font("Verdana", FontWeight.BOLD, 15));
        scenetitle1.setTextAlignment(TextAlignment.CENTER);
        scenetitle1.setTranslateX(120);
        scenetitle1.setTranslateY(40);
        scenetitle1.setFill(Color.WHITE);

        Text scenetitle = new Text("Cats Attack");
        scenetitle.setId("catsAtack-text");
        scenetitle.setFont(Font.font("Comic Sans MS", FontWeight.BOLD, 35));
        scenetitle.setTextAlignment(TextAlignment.CENTER);
        scenetitle.setTranslateX(80);
        scenetitle.setTranslateY(80);
        scenetitle.setFill(Color.WHITE);

        //imagem ilustrativa
        Image imagem = new Image("cat3.png");
        ImageView visualizadorImagem = new ImageView(imagem);
        visualizadorImagem.setFitHeight(100);
        visualizadorImagem.setFitWidth(100);
        visualizadorImagem.setTranslateX(130);
        visualizadorImagem.setTranslateY(90);

         //imagem ilustrativa
         Image menufundo = new Image("menu.png");
         ImageView visualizadorMenufundo = new ImageView(menufundo);

        //caixa texto para o nick do player
        Label userName = new Label("NAME");
        userName.setFont(Font.font("Verdana", FontWeight.BOLD, 10));
        userName.setTranslateX(84);
        userName.setTranslateY(200);
        userName.setTextFill(Color.WHITE);

        userTextField = new TextField();
        userTextField.setTranslateX(120);
        userTextField.setTranslateY(195);

        //BOTAO START
        btstart = new Button("Start");
        HBox hbBtn = new HBox(1);
        hbBtn.setAlignment(Pos.BOTTOM_CENTER);
        hbBtn.getChildren().add(btstart);
        hbBtn.setTranslateX(150);
        hbBtn.setTranslateY(230);

        btstart.setOnAction(e->trataBotaoSignIn(e));

        menuPane.getChildren().addAll(visualizadorMenufundo, visualizadorImagem,scenetitle1, scenetitle, userName,
        userTextField, hbBtn);

        scene1 = new Scene(menuPane, 350,300);
        primaryStage.setTitle(Params.WINDOW_TITLE);
        primaryStage.setResizable(false);
        primaryStage.setScene(scene1);
        primaryStage.show();
    }
    
    public void trataBotaoSignIn(ActionEvent e) {
        namePlayer = userTextField.getText();
        //para restart
        if(verificainicio==true){
        Game.getInstance().getRestart();
        }
        initGame();
    }

    public void initGame() {
        
         gamePane= new Group();
         scene2 = new Scene( gamePane );
         gameStage = new Stage();
         
 
         Canvas canvas = new Canvas(Params.WINDOW_WIDTH, Params.WINDOW_HEIGHT );
         gamePane.getChildren().add( canvas );
         
         
         // Setup Game object
         Game.getInstance().Start();
         verificainicio = true;
         //inicia controles
         initKeyControl(scene2);
 
         // Register Game Loop
         final GraphicsContext gc = canvas.getGraphicsContext2D();
 
         new AnimationTimer()
         {
             long lastNanoTime = System.nanoTime();
 
             @Override
             public void handle(long currentNanoTime)
             {
                 long deltaTime = currentNanoTime - lastNanoTime;
 
                 Game.getInstance().Update(currentNanoTime, deltaTime);
                 gc.clearRect(0, 0, Params.WINDOW_WIDTH, Params.WINDOW_HEIGHT);
                 Game.getInstance().Draw(gc);

                 gc.fillText("Pontos: "+Game.getInstance().getPontos(), 15, 20);
                 gc.fillText("Vidas: "+Game.getInstance().getVidas(), 15, 40);
                 gc.setFont(new Font(15));
                 gc.setFill(Color.BLACK);

                 if (Game.getInstance().isGameOver()){
                     stop();

                     c = new CadastroPontuacao();
                     c.cadastra(namePlayer, Game.getInstance().getPontos());
                     c.persiste();

                     gameOver();   
                 }

                 if (Game.getInstance().isGameWin()){
                    stop();

                    c = new CadastroPontuacao();
                    c.cadastra(namePlayer, Game.getInstance().getPontos());
                    c.persiste();

                    winGame();   
                }
 
                 lastNanoTime = currentNanoTime;
             }
 
         }.start();

         gameStage.setScene( scene2 );
         gameStage.initModality(Modality.APPLICATION_MODAL);
         gameStage.setTitle("Space Invaders - Attack on Cats");
         gameStage.show();
    }

    public void gameOver() {
        overPane = new Group();
        overStage = new Stage();
    
        //mensagem game over
        Text scenetitle = new Text("GAME OVER");
        scenetitle.setId("gameover-text");
        scenetitle.setFont(Font.font("Verdana", FontWeight.BOLD, 35));
        scenetitle.setTextAlignment(TextAlignment.CENTER);
        scenetitle.setTranslateX(60);
        scenetitle.setTranslateY(50);
    
        //IMAGEM ILUSTRATIVA
        Image imagem = new Image("gameover.png");
        ImageView visualizadorImagem = new ImageView(imagem);
        visualizadorImagem.setFitHeight(50);
        visualizadorImagem.setFitWidth(50);
        visualizadorImagem.setTranslateX(145);
        visualizadorImagem.setTranslateY(70);
    
        //MAIORES PONTUACOES DO JOGADOR
        c = new CadastroPontuacao();
        
        Label userName = new Label(c.toString());//chama metodo pontução talves arrumar alinhamento ----
        userName.setFont(Font.font("Verdana", FontWeight.BOLD, 10));
        userName.setTranslateX(80);
        userName.setTranslateY(150);
    
        overPane.getChildren().addAll(visualizadorImagem, scenetitle, userName);
    
        overScene = new Scene(overPane,350,300);
        overStage.setScene(overScene);
        overStage.initModality(Modality.APPLICATION_MODAL);
        overStage.setTitle("xx game-over xx");
        overStage.show();
    }

    public void winGame() {
        winPane = new Group();
        winStage = new Stage();
    
        //mensagem game over
        Text scenetitle = new Text("Win Game");
        scenetitle.setId("wingame-text");
        scenetitle.setFont(Font.font("Verdana", FontWeight.BOLD, 35));
        scenetitle.setTextAlignment(TextAlignment.CENTER);
        scenetitle.setTranslateX(80);
        scenetitle.setTranslateY(50);
    
        //IMAGEM ILUSTRATIVA
        Image imagem = new Image("girl.png");
        ImageView visualizadorImagem = new ImageView(imagem);
        visualizadorImagem.setFitHeight(50);
        visualizadorImagem.setFitWidth(50);
        visualizadorImagem.setTranslateX(145);
        visualizadorImagem.setTranslateY(70);
    
        //MAIORES PONTUACOES DO JOGADOR
        c = new CadastroPontuacao();
        
        Label userName = new Label(c.toString());//chama metodo pontução talves arrumar alinhamento ----
        userName.setFont(Font.font("Verdana", FontWeight.BOLD, 10));
        userName.setTranslateX(80);
        userName.setTranslateY(150);
    
        winPane.getChildren().addAll(visualizadorImagem, scenetitle, userName);
    
        winScene = new Scene(winPane,350,300);
        winStage.setScene(winScene);
        winStage.initModality(Modality.APPLICATION_MODAL);
        winStage.setTitle("xx win-game xx");
        winStage.show();
    }
    
    //CONTROLES
    public void initKeyControl(Scene scene) {
        // Register User Input Handler
        scene.setOnKeyPressed((KeyEvent event) -> {
           Game.getInstance().OnInput(event.getCode(), true);
       });

       scene.setOnKeyReleased((KeyEvent event) -> {
           Game.getInstance().OnInput(event.getCode(), false);
       });

   }

    public static void main(String args[]) {
        launch();
    }
}
