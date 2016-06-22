import java.util.Scanner; 

public class mixed {

	private int valueInt = 0;
	private boolean valueBool = false;
	private float valueFloat = 0;
	private char valueChar;
	private String valueString = "";

	public enum Type {
		INT_LITERAL,
		BOOLEAN_LITERAL,
		FLOAT_LITERAL,
		CHAR_LITERAL,
		STRING_LITERAL
	}

	private Type type;

	// get and set
	//get

	public static mixed add(mixed _1, mixed _2) {
		mixed newValue = new mixed();
		newValue.valueInt = _1.valueInt + _2.valueInt;
		newValue.type = Type.INT_LITERAL;
		return newValue;
	}
}
