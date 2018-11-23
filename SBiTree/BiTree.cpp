#include<stdio.h>
#include<stdlib.h>

#define OK 1;
#define ERROR -1;
#define OVERFLOW -2

typedef int TElemType;

typedef int Status;

typedef struct bt_node {
	TElemType data;
	struct bt_node *lc, *rc;
}bt_node, *BT;

typedef struct {
	BT *data;
	int front, rear;
}QQueue;
//初始化栈，为data数组在堆里分配100个节点
Status InitQQueue(QQueue *qe) {
	qe->data = (BT*)malloc(100 * sizeof bt_node);
	qe->front = qe->rear = 0;
	return OK;
}

Status Enqueue(QQueue *qe, BT b) {
	qe->data[qe->rear++] = b;
	return OK;
}

int QQueueEmpty(QQueue qe) {
	return qe.front == qe.rear;
}

Status DeQueue(QQueue *q, BT* t) {
	if (!QQueueEmpty(*q)) {
		*t = (*q).data[(*q).front];
		(*q).front++;
		return OK;
	}
	else return ERROR;
}

Status Dequeue(QQueue *qe) {
	if (!QQueueEmpty(*qe)) {
		qe->front++;
		return OK
	}
	else {
		return ERROR;
	}
}

Status QQueueFront(QQueue qe, BT *b) {
	if (!QQueueEmpty(qe)) {
		*b = qe.data[qe.front];
		return OK;
	}
	else
	{
		return OK;
	}
}

typedef struct {
	BT* bt;
	int base, top;
}SStack;

Status InitSStack(SStack *S) {
	S->bt = (BT*)malloc(100 * sizeof bt_node);
	S->base = S->top = 0;
	return OK;
}

Status push(SStack *s, BT b) {
	s->bt[s->top] = b;
	s->top++;
	return OK;
}

Status SStackEmpty(SStack s) {
	return s.base == s.top;
}

BT pop(SStack *s) {
	if (!SStackEmpty(*s)) {
		/*
			BT T = s->bt[--s->top];
		*/
		s->top--;
		BT T = s->bt[s->top];
		return T;
	}
}

BT SStackTop(SStack s) {
	if (!SStackEmpty(s))
		return s.bt[s.top - 1];
}

Status SStackTop(SStack *st, BT *b) {
	if (!SStackEmpty(*st)) {
		*b = (*st).bt[(*st).top - 1];
		return OK;
	}
	else return 0;
}

Status InitBiTree(BT* T) {
	*T = (BT)malloc(sizeof bt_node);
	(*T)->lc = (*T)->rc = NULL;
	return OK;
}

Status CreatBiTree(BT *T) {
	int i;
	scanf_s("%d", &i);
	if (i == 0) T = NULL;
	else {
		if (!(*T = (bt_node*)malloc(sizeof bt_node))) exit(OVERFLOW);
		(*T)->lc = (*T)->rc = NULL;
		(*T)->data = i;
		CreatBiTree(&(*T)->lc);
		CreatBiTree(&(*T)->rc);
	}
	return OK;
}

Status PreOrderTraverseR(BT T, int(*V)(TElemType e)) {
	if (T) {
		if (V((*T).data))
			if (PreOrderTraverseR((*T).lc, V))
				if (PreOrderTraverseR((*T).rc, V)) return OK;
		return ERROR;
	}
	else return OK;
}

Status PrintElement(TElemType e) {
	printf(" %d ", e);
	return OK;
}

Status InOrderTraverseR(BT T, int(*V)(TElemType e)) {
	if (T) {
		if (InOrderTraverseR((*T).lc, V))
			if (V((*T).data))
				if (InOrderTraverseR((*T).rc, V)) return OK;
		return ERROR;
	}
	else return OK;
}

Status PostOrderTraverseR(BT T, int(*V)(TElemType e)) {
	if (T) {
		if (PostOrderTraverseR((*T).lc, V))
			if (PostOrderTraverseR((*T).rc, V))
				if (V((*T).data)) return OK;
		return ERROR;
	}
	else return OK;
}

Status PreOrderTraverse(BT T, int(*V)(TElemType e)) {
	BT b;
	SStack s;
	InitSStack(&s);
	push(&s, T);
	while (!SStackEmpty(s)) {
		while (SStackTop(&s, &b) && b) {
			V(b->data);
			push(&s, b->lc);
		}
		b = pop(&s);
		if (!SStackEmpty(s))
		{
			b = pop(&s);
			push(&s, b->rc);
		}
	}
	return 1;
}

Status InOrderTraverse(BT T, int(*V)(TElemType e)) {
	;
	BT b;
	SStack s;
	InitSStack(&s);
	push(&s, T);
	while (!SStackEmpty(s)) {
		//while (SStackTop(&s, &b) && b->lc) {
		while (SStackTop(&s, &b) && b) {
			push(&s, b->lc);
		}
		b = pop(&s);
		if (!SStackEmpty(s))
		{
			b = pop(&s);
			V(b->data);
			//if(b->rc)
			push(&s, b->rc);
		}
	}
	return 1;
}

Status PostOrderTraverse1(BT T, int(*V)(TElemType e)) {
	SStack s;
	InitSStack(&s);
	BT b, last = T;
	push(&s, T);
	while (!SStackEmpty(s)) {
		SStackTop(&s, &b);
		//可以访问节点的三种情况
		//叶子节点：(b->lc == NULL && b->rc == NULL)
		//因为栈的的储存顺序是从右到左，所以如果最后一个访问的节点是右孩子 那么表示子节点都已经访问过了：last == b->rc 
		//还有一种可能是 该节点没有右孩子，那么我们可以用右孩子是NULL和last==左孩子来表示这种情况 (last == b->lc&&b->rc == NULL)（）
		if ((b->lc == NULL && b->rc == NULL) || last == b->rc || (last == b->lc&&b->rc == NULL)) {
			V(b->data);
			last = b;
			pop(&s);
		}
		//b的孩子按从右到左的顺序入栈
		else {
			if (b->rc) push(&s, b->rc);
			if (b->lc) push(&s, b->lc);
		}
	}
	return 1;
}

Status PostOrderTraverse2(BT T, int(*V)(TElemType)) {
	//每次把一个元素入栈两次，如果栈顶有两个相同的元素，那么表示还没操作过这个元素的孩子
	SStack s; BT b;
	InitSStack(&s);
	push(&s, T); push(&s, T);
	while (!SStackEmpty(s)) {
		b = pop(&s);
		//将弹出来的元素和栈顶元素比较，如果相等，那么依次把他的右左孩子
		//入栈，如果b和栈顶元素相等，但是他没有孩子，那么只是进入if里面，
		//但是什么都不敢（相当于从栈里消耗了一个重复的元素），到了弹出下一个栈顶元素的时候，
		//这个弹出的元素和栈顶元素肯定不相等（因为栈里面最多2个重复的元素）
		if (b == SStackTop(s)) {
			if (b->rc) { push(&s, b->rc); push(&s, b->rc); }
			if (b->lc) { push(&s, b->lc); push(&s, b->lc); }
		}
		//如果弹出来的元素和栈顶的元素不相等，那么则访问弹出来的元素
		else {
			V(b->data);
		}
	}
	return 0;
}

Status PostOrderTraverse3(BT T, int(*V)(TElemType)) {
	SStack tr, st;
	BT bt;
	InitSStack(&st); InitSStack(&tr);
	push(&st, T); push(&tr, T);
	while (!SStackEmpty(tr))
	{
		while (SStackTop(&tr, &bt) && bt) {
			push(&st, bt->rc); push(&tr, bt->rc);
		}
		pop(&st); pop(&tr); //空指针退栈
		if (!SStackEmpty(tr)) {
			bt = pop(&tr);
			push(&tr, bt->lc); push(&st, bt->lc);
		}
	}
	while (!SStackEmpty(st)) V(pop(&st)->data);
	return 0;
}


Status LevelOrderTraverse(BT T, int(*V)(TElemType)) {
	QQueue qe;
	BT bt;
	InitQQueue(&qe);
	V(T->data);
	Enqueue(&qe, T);
	while (!QQueueEmpty(qe)) {
		QQueueFront(qe, &bt);
		if (bt->lc) {
			V(bt->lc->data);
			Enqueue(&qe, bt->lc);
		}
		if (bt->rc) {
			V(bt->rc->data);
			Enqueue(&qe, bt->rc);
		}
		Dequeue(&qe);
	}
	return OK;
}
QQueue qq;
Status LevelOrderTraverseR(BT T, int(*V)(TElemType)) {
	BT b;
	if (T) {
		V(T->data);
		if (T->lc) Enqueue(&qq, T->lc);
		if (T->rc) Enqueue(&qq, T->rc);
		if (QQueueEmpty(qq)) return OK;
		DeQueue(&qq, &T);
		LevelOrderTraverseR(T, V);
	}
	else return 0;
}



int main(int argc, int **argv) {
	BT bt;
	CreatBiTree(&bt);
	InitQQueue(&qq);

	//
	//1 2 4 0 0 5 0 0 3 6 0 0 0
	//1 2 4 0 5 0 0 0 3 0 0
	//1 2 3 4 0 0 0 0 0
	printf("输入：1 2 4 0 5 0 0 0 3 0 0\n");
	printf("前序递归遍历结果：          ");
	PreOrderTraverseR(bt, PrintElement);
	printf("\n");
	printf("中序递归遍历结果：          ");
	InOrderTraverseR(bt, PrintElement);
	printf("\n");
	printf("中序非递归遍历结果：        ");
	InOrderTraverse(bt, PrintElement);
	printf("\n");
	printf("后序递归遍历结果：          ");
	PostOrderTraverseR(bt, PrintElement);
	printf("\n");
	printf("后序非递归1遍历结果：       ");
	PostOrderTraverse1(bt, PrintElement);
	printf("\n");
	printf("后序非递归2遍历结果：       ");
	PostOrderTraverse2(bt, PrintElement);
	printf("\n");
	printf("后序非递归3遍历结果：       ");
	PostOrderTraverse3(bt, PrintElement);
	printf("\n");
	printf("层序非递归遍历结果：        ");
	LevelOrderTraverse(bt, PrintElement);
	printf("\n");
	printf("层序递归遍历结果：          ");
	LevelOrderTraverseR(bt, PrintElement);
	printf("\n");
	system("pause");
	return 0;
}
