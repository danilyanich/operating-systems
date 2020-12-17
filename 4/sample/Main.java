package sample;

import controller.ExpressionTreeController;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import model.ExpressionTree;
import view.CalculatorForm;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) {
        ExpressionTree expressionTree = new ExpressionTree();
        ExpressionTreeController expressionTreeController = new ExpressionTreeController(expressionTree);
        Scene scene = new Scene(new CalculatorForm(expressionTreeController).getGridPane());

        primaryStage.setScene(scene);
        primaryStage.setTitle("Calculator");
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
