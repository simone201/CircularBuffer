/**
    Simone Renzo
    MAT. 781616
*/
#ifndef CBUFFER_H
#define CBUFFER_H       // cbuffer

#include <iostream>     // std::cout
#include <stdexcept>    // std::runtime_error

/**
    Classe empty_buffer
    Implementazione di una eccezione che indica
    il buffer vuoto, ovvero senza elementi inseriti
*/
class empty_buffer : public std::runtime_error {
public:
    /**
		Costruttore con messaggio annesso
		@param msg message error
	*/
	empty_buffer(const char *msg) : std::runtime_error(msg) {}
};

/**
    Classe index_out_of_bounds
    Implementazione di una eccezione che indica
    il richiamo di un indice oltre i limiti del buffer
*/
class index_out_of_bounds : public std::runtime_error {
public:
    /**
		Costruttore con messaggio annesso
		@param msg message error
	*/
	index_out_of_bounds(const char *msg) : std::runtime_error(msg) {}
};

/**
    Classe copy_error
    Implementazione di una eccezione che indica
    l'errore durante la copia di un cbuffer (copy constructor)
*/
class copy_error : public std::runtime_error {
public:
    /**
		Costruttore con messaggio annesso
		@param msg message error
	*/
	copy_error(const char *msg) : std::runtime_error(msg) {}
};

/**
    Classe zero_size
    Implementazione di una eccezione che indica
    una dimensione nulla del cbuffer, permessa ma non usabile
*/
class zero_size : public std::runtime_error {
public:
    /**
		Costruttore con messaggio annesso
		@param msg message error
	*/
	zero_size(const char *msg) : std::runtime_error(msg) {}
};

/**
    Classe che implementa un buffer circolare
    di grandezza definita (_size) e di tipo
    templato T. I dati vengono memorizzati
    in una lista linkata mediante nodi e puntatori.
    La classe richiede di esplicitare il tipo
    del valore di ogni nodo del buffer.
*/
template <typename T>
class cbuffer {

private:

    /**
		Struct che rappresenta un nodo del buffer.
		I dati vengono mantenuti in una singola variabile.
		Presente anche un flag per rilevare la coda del buffer.
	*/
	struct node {
		T value;        // Valore
		bool is_tail;   // Flag di coda
		node *next;     // Puntatore prossimo elemento

		/**
			Costruttore di default
		*/
		node() : value(0), is_tail(false), next(0) {}

		/**
			Distruttore
		*/
		~node() {}

		/**
			Costruttore per nodo semplice del buffer

			@param v value of the node
		*/
		node(const T &v) : value(v), is_tail(false), next(0) {}

		/**
			Costruttore per nodo semplice del buffer
			con prossimo nodo prestabilito

			@param v value of the node
			@param n pointer of next node
		*/
		node(const T &v, node *n) : value(v), is_tail(false), next(n) {}

		/**
			Costruttore per nodo di coda

			@param it is_tail flag
		*/
		node(const bool &it) : value(0), is_tail(it), next(0) {}

		/**
			Copy constructor

			@param other cbuffer to be copied
		*/
		node(const node &other) : value(other.value), is_tail(other.is_tail), next(other.next) {}

		/**
			Operatore di assegnamento

			@param other cbuffer to be copied
		*/
		node&operator=(const node &other) {
			if(this != &other) {
				value = other.value;
				is_tail = other.is_tail;
				next = other.next;
			}

			return *this;
		}
	}; // struct node

	node *_head;    // puntatore in testa al buffer
	node *_curpos;  // puntatore alla posizione corrente
	node *_tail;    // puntatore in coda al buffer

	int _size;      // grandezza massima del buffer
	int _cursize;   // grandezza attuale del buffer

protected:

	/**
		Costruttore di default.
		Nascosto per evitare il suo utilizzo.

		@throw zero_size if called the buffer cannot be zero
	*/
	cbuffer() {
        throw zero_size("The buffer size cannot be zero");
	}

public:

	/**
		Distruttore
	*/
	~cbuffer() {
		clear();
	}

	/**
		Costruttore con grandezza del buffer circolare

		@param s cbuffer size
		@throw zero_size if the passed size is less or equal to zero
	*/
	cbuffer(const int &s) : _head(0), _curpos(0), _tail(0), _size(s), _cursize(0) {
	    if(s <= 0)
            throw zero_size("The buffer size cannot be zero or less");
	}

	/**
        Copy constructor

		@param other cbuffer to be copied
		@throw zero_size if the other buffer has zero size
		@throw empty_buffer if the other buffer has no nodes
	*/
	cbuffer(const cbuffer &other) : _head(0), _curpos(0), _tail(0), _size(0), _cursize(0) {
	    if(other.get_max_size() == 0)
            throw zero_size("The other cbuffer has size zero");
	    if(other.get_current_size() == 0)
            throw empty_buffer("The other cbuffer has zero items");

        node *temp = other._head;

        _head = new node(temp->value);
        _tail = new node(true);

        _tail->next = _head;
        _curpos = _head;
        _size = other.get_max_size();

        temp = temp->next;
        _cursize++;

		try {
			while (temp != other._tail) {
				add(temp->value);
				temp = temp->next;
			}

			_curpos->next = _tail;
		} catch(...) {
			clear();
			throw copy_error("Runtime Error during object copy");
		}
	}

	/**
		Operatore di assegnamento

		@param other cbuffer to be copied
		@return *this
	*/
	cbuffer& operator=(const cbuffer &other) {

		if (this != &other)
			cbuffer temp(other);

		return *this;
	}

	/**
		Operatore di accesso diretto al buffer

		@param pos position in the buffer
		@return T
		@throw index_out_of_bounds if the position is not valid
	*/
    T &operator[](const int pos) {
        if(pos < _cursize) {
            node* _supp = _head;

            for(int i = 0; i != pos; i++)
                _supp = _supp->next;

            return _supp->value;
        } else
            throw index_out_of_bounds("Index is not valid");
    }

    /**
		Inserisce in coda al buffer un nuovo nodo di valore v.

		@param v value
	*/
	void add(const T &v) {
        if(_head == 0 || _head == _tail) {
            _head = new node(v);
            _curpos = _head;

            if(_tail == 0)
                _tail = new node(true);

            _curpos->next = _tail;
            _tail->next = _head;

            _cursize++;
        } else {
            node *tmp = new node(v);

            if(_cursize == _size) {
                node* app = _curpos;
                _curpos = _head;
                _head = _head->next;

                delete _curpos;
                _curpos = tmp;
                _curpos->next = _tail;
                _tail->next = _head;
                app->next = _curpos;
            } else {
                _cursize++;
                _curpos->next = tmp;
                _curpos = tmp;
                _curpos->next = _tail;
                _tail->next = _head;
            }
        }
    }

    /**
		Rimuove in testa al buffer un nodo e ri-linka
		i puntatori in modo corretto.

		@throw empty_buffer in case of empty buffer
	*/
    void remove() {
        if(_head == 0 || _cursize == 0)
            throw empty_buffer("No elements in the buffer");

        node *temp = _head->next;
        delete _head;
        _head = temp;
        _tail->next = _head;
        _cursize--;
    }

    /**
        Ritorna la grandezza attuale del buffer.
    */
	int get_current_size() const {
		return _cursize;
	}

	/**
        Ritorna la grandezza massima del buffer.
    */
	int get_max_size() const {
		return _size;
	}

	/**
        Metodo che svuota e dealloca il buffer.
    */
	void clear() {
	    node *temp = _head;
        node *temp2 = 0;

        if(_cursize == 0 && _head != 0) {
            delete _head;
        } else {
            for(int i = 0; i < _cursize; i++) {
                temp2 = temp->next;
                delete temp;
                temp = temp2;
            }

            delete temp2;
        }
	}

	/**
        Forward iterator di sola lettura del buffer.
	 */
	class iterator {
	public:
		typedef std::forward_iterator_tag 		iterator_category;  // iteratore forward
		typedef T                 				value_type;         // ritorno dell'iteratore
		typedef std::ptrdiff_t                 	difference_type;    // differenza tra puntatori
		typedef const T*                		pointer;            // tipo di puntatore
		typedef const T&                		reference;          // tipo di reference

		/**
			Costruttore di default
		*/
		iterator() {
			n = 0;
		}

		/**
			Copy constructor
			@param other iterator to be copied
		*/
		iterator(const iterator &other) {
			n = other.n;
		}

		/**
			Operatore di assegnamento
			@param other iterator to be copied
			@return *this
        */
		iterator& operator=(const iterator &other) {
			n = other.n;
			return *this;
		}

		/**
			Distruttore
        */
		~iterator() {}

		/**
			Dereferenziamento per reference
			@return const reference to node value
		*/
		const T& operator*() const {
			return n->value;
		}

		/**
			Dereferenziamento per puntatore
			@return const pointer to node value
        */
		const T* operator->() const {
			return &(n->value);
		}

		/**
			Post-incremento
			@return iterator at previous node
        */
		iterator operator++(int) {
			iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		/**
			Pre-incremento
			@return iterator at next node
        */
		iterator& operator++() {
			n = n->next;
			return *this;
		}

		/**
			Confronto (uguaglianza)
			@param other iterator to be checked
			@return true if both iterators point to the same position
        */
		bool operator==(const iterator &other) const {
			return n == other.n;
		}

		/**
			Confronto (diversita')
			@param other iterator to be checked
			@return false if both iterators point to the same position
        */
		bool operator!=(const iterator &other) const {
			return n != other.n;
		}

	private:
		const node *n;          // nodo in lettura

		friend class cbuffer;   // permette di sfruttare la classe dall'esterno

		/**
			Costruttore secondario di inizializzazione
			@param pn pointer to a buffer node
		*/
		iterator(const node *pn) {
			n = pn;
		}

	}; // class iterator

	/**
        Iteratore di inizio buffer.

        @return begin buffer iterator
	*/
	iterator begin() const {
		return iterator(_head);
	}

	/**
        Iteratore di fine buffer.

        @return end buffer iterator
	*/
	iterator end() const {
		return iterator(_tail);
	}

}; // class cbuffer

/**
	Operatore di stream di output per la stampa di un cbuffer

	@param os output stream
	@param cb const cbuffer to be printed
	@return output stream
*/
template <typename T>
std::ostream &operator<<(std::ostream &os, const cbuffer<T> &cb) {
	typename cbuffer<T>::iterator i, ie;

	for(i = cb.begin(), ie = cb.end(); i != ie; i++)
		os << "<" << *i << ">";

	return os;
}

/**
	Metodo check che applica una funzione p a singolo parametro
	ad ogni elemento di cbuffer e stampa a video i risultati.
	Accetta solo funzioni con ritorno booleano e stampa true/false
	in base alla verita' di ritorno in base all'indice in lettura.
	Il buffer originario viene copiato per evitare di modificarlo
	cosi' mantenendo una integrita' dei dati nel metodo stesso.

	@param cb const cbuffer to be read
	@param p function pointer with single parameter T
*/
template <typename T>
void check(const cbuffer<T> &cb, bool (*p)(T)) {
    if(cb.get_current_size() > 0) {
        cbuffer<T> temp = cb;   // Mantiene invariato il cbuffer originario, lavora su temp

        for(int i = 0; i < cb.get_current_size(); i++) {
            if(p(temp[i]))
                std::cout << "[" << i << "]: true" << std::endl;
            else
                std::cout << "[" << i << "]: false" << std::endl;
        }
    }
}

#endif
