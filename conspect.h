void *alloc(int num_bytes); 

void free (void *ptr);  //указатель на void - указатель на неопределённый тип

void Dump(); 
//print statistic
//кол-во занятых и свободных блоков, их общий объём, наибольший непрерывный кусок памяти

struct MemoryControlBlock {
	bool Free;
	int size;
	//указатель на следующий блок? И на предыдущий, чтобы можно было объединить соседние
}

void free(void *ptr) {
	ptr->Free = 0;  //Что за стрелочка??
}

void *first;  //allocate heap by sys call (malloc) ИЛИ = new [1000];

void *alloc(int num_bytes){
	void *ret = first;
	first += num_bytes;
	return ret;
}

struct MapItem{
	int number;
	int count;
};

int main(){
//подменить системный аллокатор
	void *array = new int[10000];
	for (int i = 0; i<10000; i++){
		array[i] = alloc(sizeof(MapItem));
	}
	Dump();
	for (int i = 0; i<10000; i++){
		free(array[i]);
	}
	Dump();
}

//должен быть контроль за общим количеством памяти
//системный вызов для выделения куска памяти
//в файле - мн-во чисел, их складываем в map: число-количество вхождений
//map свой, либо модифицированный, чтобы использовался свой аллокатор
//Бонус: контроль фрагментации
//Сначала один большой блок, потом делим его на кусочки