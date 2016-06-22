import java.io.PrintStream;

public class Mixed {

	static PrintStream out = System.out; 

	private int valueInt = 0;
	private boolean valueBool = false;
	private float valueFloat = 0;
	private char valueChar;
	private String valueString = "";

	public static enum Type {
		INT_LITERAL,
		BOOLEAN_LITERAL,
		FLOAT_LITERAL,
		CHAR_LITERAL,
		STRING_LITERAL
	}

	private Type type;

	public Mixed () {
		//
	}

	public static void main(String[] args) {
		out.print(1 );
	}

	public static Mixed setInt(int value) {
		Mixed mix = new Mixed();
		mix.valueInt = value;
		mix.type = Type.INT_LITERAL;
		return mix;
	}

	public static Mixed setBool(int value) {
		Mixed mix = new Mixed();
		mix.valueBool = value != 0;
		mix.type = Type.BOOLEAN_LITERAL;
		return mix;
	}

	public static Mixed setFloat(float value) {
		Mixed mix = new Mixed();
		mix.valueFloat = value;
		mix.type = Type.FLOAT_LITERAL;
		return mix;
	}

	public static Mixed setString(String value) {
		Mixed mix = new Mixed();
		mix.valueString = value;
		mix.type = Type.STRING_LITERAL;
		return mix;
	}

	public int getInt() {
		return valueInt;
	}

	public int getBoolean() {
		return valueBool ? 1 : 0;
	}

	public float getFloat() {
		return valueFloat;
	}

	public String getString() {
		return valueString;
	}

	public static Mixed mixedAdd(Mixed _1, Mixed _2) {
		if( _1.type == Type.INT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				_1.valueInt = _1.valueInt + _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				_1.valueFloat = _1.valueInt + _2.valueFloat;
				_1.type = Type.FLOAT_LITERAL;
			} else if( _2.type == Type.STRING_LITERAL) {
				_1.valueString = _1.valueInt + _2.valueString;
				_1.type = Type.STRING_LITERAL;
			}
		} else if( _1.type == Type.FLOAT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				_1.valueFloat = _1.valueFloat + _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				_1.valueFloat = _1.valueFloat + _2.valueFloat;
			} else if( _2.type == Type.STRING_LITERAL) {
				_1.valueString = _1.valueFloat + _2.valueString;
				_1.type = Type.STRING_LITERAL;
			}
		} else if( _1.type == Type.STRING_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				_1.valueString = _1.valueString + _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				_1.valueString = _1.valueString + _2.valueFloat;
			} else if( _2.type == Type.STRING_LITERAL) {
				_1.valueString = _1.valueString + _2.valueString;
			}
		}
		return _1;
	}

	public static Mixed mixedSub(Mixed _1, Mixed _2) {
		if( _1.type == Type.INT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				_1.valueInt = _1.valueInt - _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				_1.valueFloat = ((float)_1.valueInt) - _2.valueFloat;
				_1.type = Type.FLOAT_LITERAL;
			}
		} else if( _1.type == Type.FLOAT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				_1.valueFloat = _1.valueFloat - ((float)_2.valueInt);
			} else if( _2.type == Type.FLOAT_LITERAL) {
				_1.valueFloat = _1.valueFloat - _2.valueFloat;
			}
		}
		return _1;
	}

	public static Mixed mixedMul(Mixed _1, Mixed _2) {
		if( _1.type == Type.INT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				_1.valueInt = _1.valueInt * _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				_1.valueFloat = _1.valueInt * _2.valueFloat;
				_1.type = Type.FLOAT_LITERAL;
			}
		} else if( _1.type == Type.FLOAT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				_1.valueFloat = _1.valueFloat * _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				_1.valueFloat = _1.valueFloat * _2.valueFloat;
			}
		}
		return _1;
	}

	public static Mixed mixedEqually(Mixed _1, Mixed _2) {
		_1.valueBool = (_1.valueBool == _2.valueBool);
		_1.type = Type.BOOLEAN_LITERAL;
		return _1;
	}

	public static Mixed mixedNotEqually(Mixed _1, Mixed _2) {
		_1.valueBool = (_1.valueBool != _2.valueBool);
		_1.type = Type.BOOLEAN_LITERAL;
		return _1;
	}

	public static Mixed mixedNot(Mixed _1) {
		_1.valueBool = (!_1.valueBool);
		_1.type = Type.BOOLEAN_LITERAL;
		return _1;
	}

	public static Mixed mixedMore(Mixed _1, Mixed _2) {
		boolean isOk = false;
		if( _1.type == Type.INT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				isOk = _1.valueInt > _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				isOk = _1.valueInt > _2.valueFloat;
			}
		} else if( _1.type == Type.FLOAT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				isOk = _1.valueFloat > ((float)_2.valueInt);
			} else if( _2.type == Type.FLOAT_LITERAL) {
				isOk = _1.valueFloat > _2.valueFloat;
			}
		}
		_1.valueBool = isOk;
		_1.type = Type.BOOLEAN_LITERAL;
		return _1;
	}

	public static Mixed mixedLess(Mixed _1, Mixed _2) {
		boolean isOk = false;
		if( _1.type == Type.INT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				isOk = _1.valueInt < _2.valueInt;
			} else if( _2.type == Type.FLOAT_LITERAL) {
				isOk = _1.valueInt < _2.valueFloat;
			}
		} else if( _1.type == Type.FLOAT_LITERAL) {
			if( _2.type == Type.INT_LITERAL) {
				isOk = _1.valueFloat < ((float)_2.valueInt);
			} else if( _2.type == Type.FLOAT_LITERAL) {
				isOk = _1.valueFloat < _2.valueFloat;
			}
		}
		_1.valueBool = isOk;
		_1.type = Type.BOOLEAN_LITERAL;
		return _1;
	}

	public static void mixedPrint(Mixed mix) {
		if (mix.type == Type.INT_LITERAL )	out.print(mix.valueInt + "\n");
		if (mix.type == Type.BOOLEAN_LITERAL )	out.print(mix.valueBool + "\n");
		if (mix.type == Type.FLOAT_LITERAL )	out.print(mix.valueFloat + "\n");
		if (mix.type == Type.STRING_LITERAL )	out.print(mix.valueString + "\n");
	}
}
