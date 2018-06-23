typedef struct vector VECTOR;
typedef struct matrix MATRIX;
typedef struct camera CAMERA;
typedef struct object OBJECT;
typedef struct face3d FACE3D;
typedef struct vertex VERTEX;

typedef void (*DrawF) (FACE3D *,char *,int);
typedef void (*CompC) (FACE3D *);

struct vector{
	float	v1;
	float	v2;
	float	v3;
};

struct matrix{
	float 	m11,m12,m13;
	float 	m21,m22,m23;
	float 	m31,m32,m33;
};

struct camera{
	VECTOR	Position;
	VECTOR	Rotation;
	float	FOV;
	float	Roll;
	MATRIX	Transform;
	MATRIX	InvTrn;
	MATRIX	Final;
};

struct	vertex{
	VECTOR	o;
	VECTOR	t;
	VECTOR	p;
	VERTEX	*Next;
};

struct face3d{
	int	NVertex;
	VERTEX	*vl;
	float	Color;
        FACE3D  *Next;
};

struct object{
	VECTOR	Position;
	VECTOR	Rotation;
	DrawF	DrawFace;
	CompC	ComputeColor;
	int	NFaces;
        FACE3D  *fl;                    //FaceList
	MATRIX	Transform;
	MATRIX	Final;
	int	Color;
};
