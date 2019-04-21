package engine.util.math.vector;

public class Matrix {
	
	private static final float TOLERANCE = 0.00001F;
	
	private final int size;
	
	private float[][] A;
	
	public Matrix(int size) {
		
		if(size <= 0)
			throw new IllegalArgumentException("Matrix size must be positive.");
		
		this.size = size;
		A = new float[size][size];
	}
	
	public Matrix(Matrix m) {
		this(m.size);
		load(m);
	}
	
	public Matrix(float[][] a) {
		this(a.length);
		load(a);
	}
	
	public Matrix(float... a) {
		this((int) Math.sqrt(a.length));
		load(a);
	}
	
	public float getValue(int row, int column) { return A[row][column]; }
	
	public Matrix setValue(int row, int column, float value) {
		A[row][column] = value;
		return this;
	}
	
	public int getSize() { return size; }
	
	public Matrix add(Matrix m) {
		
		checkCompatibility(size, m.size);
		
		for(int y = 0; y < size; y++) {
			for(int x = 0; x < size; x++) {
				A[y][x] += m.A[y][x];
			}
		}
		return this;
	}
	
	public Matrix sub(Matrix m) {
		
		checkCompatibility(size, m.size);
		
		for(int y = 0; y < size; y++) {
			for(int x = 0; x < size; x++) {
				A[y][x] -= m.A[y][x];
			}
		}
		return this;
	}
	
	public Matrix mul(Matrix m) {
		return load(mul(m, this));
	}
	
	public Matrix mul(float s) {
		
		for(int y = 0; y < size; y++) {
			for(int x = 0; x < size; x++) {
				A[y][x] *= s;
			}
		}
		return this;
	}
	
	public Matrix negate() {
		return mul(-1.0F);
	}
	
	public Matrix div(float s) {
		
		for(int y = 0; y < size; y++) {
			for(int x = 0; x < size; x++) {
				A[y][x] /= s;
			}
		}
		return this;
	}
	
	public Matrix setIdentity() {
		
		for(int y = 0; y < size; y++) {
			for(int x = 0; x < size; x++) {
				A[y][x] = x == y ? 1 : 0;
			}
		}
		return this;
	}
	
	public Matrix transpose() {
		
		for(int y = 0; y < size; y++) {
			for(int x = y + 1; x < size; x++) {
				
				float swap = A[y][x];
				A[y][x] = A[x][y];
				A[x][y] = swap;
			}
		}
		return this;
	}
	
	public Matrix invert() {
		return load(Matrix.adjugate(this).div(det()));
	}
	
	public float det() { return detRecurse(A, 0); }
	
	private float detRecurse(float[][] a, int x) {
		
		if(a.length == 1) return a[0][x];
		
		float det = 0;
		
		float[][] b = new float[a.length - 1][a.length];
		
		for(int y = 0; y < b.length; y++)
			b[y] = a[y + 1];
		
		for(int y = 0; y <= b.length; y++) { 
			
			float sign = y % 2 == 0 ? 1.0F : -1.0F;
			det += a[y][x] * detRecurse(b, x + 1) * sign;
			
			if(y < b.length)
				b[y] = a[y];
		}
		return det;
	}
	
	public Matrix load(Matrix m) { return load(m.A); }
	
	public Matrix load(float[][] a) {
		
		checkCompatibility(size, a.length);
		
		if(a.length != a[0].length)
			throw new IllegalArgumentException("Matrix must be a square.");
		
		for(int i = 0; i < size; i++) {
			A[i] = a[i].clone();
		}
		return this;
	}
	
	public Matrix load(float... a) {
		
		if(Math.sqrt(a.length) != Math.floor(Math.sqrt(a.length)))
			throw new IllegalArgumentException("Matrix must be a square.");
		
		checkCompatibility(size, (int) Math.sqrt(a.length));
		
		for(int i = 0; i < size * size; i++) {
			A[i / size][i % size] = a[i];
		}
		return this;
	}
	
	@Override
	public Matrix clone() { return new Matrix(this); }
	
	public float[][] asArray() {
		float[][] a = new float[size][];
		for(int y = 0; y < size; y++) {
			a[y] = A[y].clone();
		}
		return a;
	}
	
	@Override
	public boolean equals(Object o) {
		if(!(o instanceof Matrix)) return false;
		else return equal(this, (Matrix) o);
	}
	
	@Override
	public String toString() {
		
		String str = "";
		
		for(int y = 0; y < size; y++) {
			
			str += "[";
			
			for(int x = 0; x < size; x++) {
				str += A[y][x] + (x != size - 1 ? ", " : "]\n");
			}
		}
		return str;
	}
	
	public static Matrix add(Matrix m1, Matrix m2) {
		return m1.clone().add(m2);
	}
	
	public static Matrix sub(Matrix m1, Matrix m2) {
		return m1.clone().sub(m2);
	}
	
	public static Matrix mul(Matrix m1, Matrix m2) {
		
		checkCompatibility(m1.size, m2.size);
		
		Matrix p = new Matrix(m1.size);
		
		for(int y = 0; y < p.size; y++) {
			
			for(int x = 0; x < p.size; x++) {
				
				for(int i = 0; i < p.size; i++) {
					
					p.A[y][x] += m1.A[y][i] * m2.A[i][x];
				}
			}
		}
		return p;
	}
	
	public static Matrix mul(Matrix... m) {
		
		Matrix p = new Matrix(m[0].size).setIdentity();
		
		for(int i = 0; i < m.length; i++) {
			p = mul(p, m[i]);
		}
		return p;
	}
	
	public static Matrix mul(Matrix m, float s) {
		return m.clone().mul(s);
	}
	
	public static Matrix negate(Matrix m) {
		return m.clone().negate();
	}
	
	public static Matrix div(Matrix m, float s) {
		return m.clone().div(s);
	}
	
	public static Matrix transpose(Matrix m) {
		return m.clone().transpose();
	}
	
	public static Matrix invert(Matrix m) {
		return m.clone().invert();
	}
	
	public static Matrix minor(Matrix m, int x, int y) {
		
		float[][] a = new float[m.size - 1][m.size - 1];
		
		for(int yy = 0; yy < m.size - 1; yy++) {
			for(int xx = 0; xx < m.size - 1; xx++) {
				
				a[yy][xx] = m.A[yy < y ? yy : yy + 1][xx < x ? xx : xx + 1];
			}
		}
		return new Matrix(a);
	}
	
	public static Matrix cofactor(Matrix m) {
		
		float[][] a = new float[m.size][m.size];
		
		for(int y = 0; y < m.size; y++) {
			for(int x = 0; x < m.size; x++) {
				
				float sign = (x + y) % 2 == 0 ? 1.0F : -1.0F;
				a[y][x] = minor(m, x, y).det() * sign;
			}
		}
		return new Matrix(a);
	}
	
	public static Matrix adjugate(Matrix m) {
		return transpose(cofactor(m));
	}
	
	public static boolean equal(Matrix m1, Matrix m2) {
		
		if(m1.size != m2.size) return false;
		
		for(int y = 0; y < m1.size; y++) {
			for(int x = 0; x < m1.size; x++) {
				
				if(Math.abs(m2.A[y][x] - m1.A[y][x]) > TOLERANCE)
					return false;
			}
		}
		return true;
	}
	
	private static void checkCompatibility(int size1, int size2) {
		if(size1 != size2)
			throw new IllegalArgumentException("Matrices must be of equal size.");
	}
}