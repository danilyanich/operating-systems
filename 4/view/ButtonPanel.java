package view;

import controller.ExpressionTreeController;
import controller.ExpressionTransformer;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.TextField;
import javafx.scene.layout.*;

import model.OperatorSelection;


public class ButtonPanel {
    public static final String CLEAR = "C";
    public static final String BACKSPACE = "←";
    public static final String EQUAL = "=";
    public static final String REVERSE = "1/x";
    public static final String OPEN = "(";
    public static final String CLOSE = ")";

    public static final String ONE = "1";
    public static final String TWO = "2";
    public static final String THREE = "3";
    public static final String FOUR = "4";
    public static final String FIVE = "5";
    public static final String SIX = "6";
    public static final String SEVEN = "7";
    public static final String EIGHT = "8";
    public static final String NINE = "9";
    public static final String ZERO = "0";

    private static final String BUTTON_STYLE =
            "-fx-pref-width: 50; " +
                    "-fx-pref-height: 50; " +
                    "-fx-font-size: 14;";


    private TextField expRowTextField;
    private TreePanel expressionTreePanel;

    private GridPane gridPane;

    private ExpressionTreeController expressionTreeController;


    public ButtonPanel(RowPanel expressionRowPanel, TreePanel expressionTreePanel,ExpressionTreeController expressionTreeController)
    {
        expRowTextField = expressionRowPanel.getExpressionRowTextField();
        this.expressionTreePanel = expressionTreePanel;
        gridPane = new GridPane();
        configureGridPane();
        this.expressionTreeController = expressionTreeController;
    }

    public GridPane getGridPane() {
        return gridPane;
    }



    private void configureGridPane() {

        Button reverse = new Button(REVERSE);
        Button sqrt = new Button(OperatorSelection.SQRT);
        Button plus = new Button(OperatorSelection.PLUS);
        Button minus = new Button(OperatorSelection.MINUS);
        Button divide = new Button(OperatorSelection.DIVISION);
        Button multiplication = new Button(OperatorSelection.MULTIPLICATION);
        Button mod = new Button(OperatorSelection.MOD);

        Button openBracket = new Button(OPEN);
        Button closeBracket = new Button(CLOSE);
        Button clear = new Button(CLEAR);
        Button backspace = new Button(BACKSPACE);
        Button dot = new Button(ExpressionTransformer.DOT);
        Button equal = new Button(EQUAL);

        Button one = new Button(ONE);
        Button two = new Button(TWO);
        Button three = new Button(THREE);
        Button four = new Button(FOUR);
        Button five = new Button(FIVE);
        Button six = new Button(SIX);
        Button seven = new Button(SEVEN);
        Button eight = new Button(EIGHT);
        Button nine = new Button(NINE);
        Button zero = new Button(ZERO);

        Button sin = new Button(OperatorSelection.SIN);
        Button cos = new Button(OperatorSelection.COS);
        Button tan = new Button(OperatorSelection.TAN);
        Button asin = new Button(OperatorSelection.ASIN);
        Button acos = new Button(OperatorSelection.ACOS);
        Button atan = new Button(OperatorSelection.ATAN);
        Button atan2 = new Button(OperatorSelection.ATAN2);


        createButton(reverse, sqrt, plus, minus, divide, multiplication, mod, openBracket, closeBracket,
                clear, backspace, dot, equal, one, two, reverseEventHandler, sqrtEventHandler, plusEventHandler,
                minusEventHandler, divideEventHandler, multiplicationEventHandler, modEventHandler, openBracketEventHandler,
                closeBracketEventHandler, clearEventHandler, backspaceEventHandler, dotEventHandler, equalEventHandler,
                oneEventHandler, twoBracketEventHandler);

        createButton(three, four, five, six, seven, eight, nine, zero, sin, cos, tan, asin, acos, atan, atan2,
                threeEventHandler, fourEventHandler, fiveEventHandler, sixEventHandler, sevenEventHandler, eightEventHandler,
                nineEventHandler, zeroEventHandler, sinEventHandler, cosEventHandler, tanEventHandler, asinEventHandler,
                acosEventHandler, atanEventHandler, atan2EventHandler);

        buttonStyle(reverse, sqrt, plus, minus, divide, multiplication, mod, openBracket, closeBracket, clear,
                     backspace, dot, equal, one, two);

        buttonStyle(three, four, five, six, seven, eight, nine, zero, atan2, sin, cos, tan, asin, acos, atan);



        equal.setStyle("-fx-pref-width: 50;" +
                "-fx-pref-height: 250");

        zero.setStyle("-fx-pref-width: 150;" +
                "-fx-pref-height: 50;");


        CheckBox trigonometryOperationActivator = new CheckBox("Trigonometric functions");
        trigonometryOperationActivator.setSelected(true);
        trigonometryOperationActivator.setOnAction(e -> {
            sin.setDisable(!trigonometryOperationActivator.isSelected());
            cos.setDisable(!trigonometryOperationActivator.isSelected());
            tan.setDisable(!trigonometryOperationActivator.isSelected());
            asin.setDisable(!trigonometryOperationActivator.isSelected());
            acos.setDisable(!trigonometryOperationActivator.isSelected());
            atan.setDisable(!trigonometryOperationActivator.isSelected());
            atan2.setDisable(!trigonometryOperationActivator.isSelected());
        });



        gridPane.add(trigonometryOperationActivator, 0,0, 6, 1);

        gridPane.add(sin, 0,1);
        gridPane.add(cos, 1,1);
        gridPane.add(tan, 2,1);
        gridPane.add(asin, 3,1);
        gridPane.add(acos, 4,1);
        gridPane.add(atan2, 4,2);
        gridPane.add(atan, 5,1);

        gridPane.add(backspace, 0,2);
        gridPane.add(clear, 1,2);
        gridPane.add(openBracket, 2,2);
        gridPane.add(closeBracket, 3,2);
        gridPane.add(sqrt, 4,3);

        gridPane.add(seven, 0,3);
        gridPane.add(eight, 1,3);
        gridPane.add(nine, 2,3);
        gridPane.add(divide, 3,3);
        gridPane.add(mod, 4,4);

        gridPane.add(four, 0,4);
        gridPane.add(five, 1,4);
        gridPane.add(six, 2,4);
        gridPane.add(multiplication, 3,4);
        gridPane.add(reverse, 4,5);


        gridPane.add(one, 0,5);
        gridPane.add(two, 1,5);
        gridPane.add(three, 2,5);
        gridPane.add(plus, 3,5);
        gridPane.add(minus, 4,6);


        gridPane.add(zero, 0,6,3,1);
        gridPane.add(dot, 3,6);


        gridPane.add(equal, 5,2, 1, 5);


        GridPane.setMargin(trigonometryOperationActivator, new Insets(15));
    }

    public void buttonStyle(Button reverse, Button sqrt, Button plus, Button minus, Button divide, Button multiplication,
                            Button mod, Button openBracket, Button closeBracket, Button clear, Button backspace, Button dot,
                            Button equal, Button one, Button two)
    {
        reverse.setStyle(BUTTON_STYLE);
        sqrt.setStyle(BUTTON_STYLE);
        plus.setStyle(BUTTON_STYLE);
        minus.setStyle(BUTTON_STYLE);
        divide.setStyle(BUTTON_STYLE);
        multiplication.setStyle(BUTTON_STYLE);
        mod.setStyle(BUTTON_STYLE);
        openBracket.setStyle(BUTTON_STYLE);
        closeBracket.setStyle(BUTTON_STYLE);
        clear.setStyle(BUTTON_STYLE);
        backspace.setStyle(BUTTON_STYLE);
        dot.setStyle(BUTTON_STYLE);
        equal.setStyle(BUTTON_STYLE);
        one.setStyle(BUTTON_STYLE);
        two.setStyle(BUTTON_STYLE);
    }

    private void createButton(Button reverse, Button sqrt, Button plus, Button minus, Button divide, Button multiplication,
                              Button mod, Button openBracket, Button closeBracket, Button clear, Button backspace, Button dot,
                              Button equal, Button one, Button two, EventHandler<ActionEvent> reverseEventHandler,
                              EventHandler<ActionEvent> sqrtEventHandler, EventHandler<ActionEvent> plusEventHandler,
                              EventHandler<ActionEvent> minusEventHandler, EventHandler<ActionEvent> divideEventHandler,
                              EventHandler<ActionEvent> multiplicationEventHandler, EventHandler<ActionEvent> modEventHandler,
                              EventHandler<ActionEvent> openBracketEventHandler, EventHandler<ActionEvent> closeBracketEventHandler,
                              EventHandler<ActionEvent> clearEventHandler, EventHandler<ActionEvent> backspaceEventHandler,
                              EventHandler<ActionEvent> dotEventHandler, EventHandler<ActionEvent> equalEventHandler,
                              EventHandler<ActionEvent> oneEventHandler, EventHandler<ActionEvent> twoBracketEventHandler)
    {
        reverse.setOnAction(reverseEventHandler);
        sqrt.setOnAction(sqrtEventHandler);
        plus.setOnAction(plusEventHandler);
        minus.setOnAction(minusEventHandler);
        divide.setOnAction(divideEventHandler);
        multiplication.setOnAction(multiplicationEventHandler);
        mod.setOnAction(modEventHandler);
        openBracket.setOnAction(openBracketEventHandler);
        closeBracket.setOnAction(closeBracketEventHandler);
        clear.setOnAction(clearEventHandler);
        backspace.setOnAction(backspaceEventHandler);
        dot.setOnAction(dotEventHandler);
        equal.setOnAction(equalEventHandler);
        one.setOnAction(oneEventHandler);
        two.setOnAction(twoBracketEventHandler);
    }


    private EventHandler<ActionEvent> oneEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + ONE);
    };

    private EventHandler<ActionEvent> twoBracketEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + TWO);
    };

    private EventHandler<ActionEvent> threeEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + THREE);
    };

    private EventHandler<ActionEvent> fourEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + FOUR);
    };

    private EventHandler<ActionEvent> fiveEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + FIVE);
    };

    private EventHandler<ActionEvent> sixEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + SIX);
    };

    private EventHandler<ActionEvent> sevenEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + SEVEN);
    };

    private EventHandler<ActionEvent> eightEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + EIGHT);
    };

    private EventHandler<ActionEvent> nineEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + NINE);
    };

    private EventHandler<ActionEvent> zeroEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + ZERO);
    };

    private EventHandler<ActionEvent> openBracketEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OPEN);
    };

    private EventHandler<ActionEvent> closeBracketEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + CLOSE);
    };

    private EventHandler<ActionEvent> clearEventHandler = e -> {
        expRowTextField.setText("");
    };

    private EventHandler<ActionEvent> backspaceEventHandler = e -> {
        if (!expRowTextField.getText().equals("")) {
            expRowTextField.setText(expRowTextField.getText().substring(0, expRowTextField.getText().length() - 1));
        }
    };

    private EventHandler<ActionEvent> dotEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + ExpressionTransformer.DOT);
    };

    private EventHandler<ActionEvent> equalEventHandler = e -> {
        try {
            expressionTreeController.createTree(expRowTextField.getText());
        } catch (Exception ex) {
            expRowTextField.setText(ex.getMessage());
            return;
        }

        expressionTreePanel.construct();
    };

    private EventHandler<ActionEvent> reverseEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + REVERSE.substring(0, 2));
    };

    private EventHandler<ActionEvent> sqrtEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.SQRT);
    };

    private EventHandler<ActionEvent> plusEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.PLUS);
    };

    private EventHandler<ActionEvent> minusEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.MINUS);
    };

    private EventHandler<ActionEvent> divideEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.DIVISION);
    };

    private EventHandler<ActionEvent> multiplicationEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.MULTIPLICATION);
    };

    private EventHandler<ActionEvent> modEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.MOD);
    };


    private EventHandler<ActionEvent> sinEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.SIN + OPEN);
    };

    private EventHandler<ActionEvent> cosEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.COS + OPEN);
    };

    private EventHandler<ActionEvent> tanEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.TAN + OPEN);
    };
    private EventHandler<ActionEvent> asinEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.ASIN + OPEN);
    };

    private EventHandler<ActionEvent> acosEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.ACOS + OPEN);
    };

    private EventHandler<ActionEvent> atanEventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.ATAN + OPEN);
    };

    private EventHandler<ActionEvent> atan2EventHandler = e -> {
        expRowTextField.setText(expRowTextField.getText() + OperatorSelection.ATAN2 + OPEN);
    };
}
