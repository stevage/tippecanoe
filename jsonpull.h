typedef enum json_type {
	// These types can be returned by json_read()
	JSON_HASH,
	JSON_ARRAY,
	JSON_NUMBER,
	JSON_STRING,
	JSON_TRUE,
	JSON_FALSE,
	JSON_NULL,

	// These and JSON_HASH and JSON_ARRAY can be called back by json_read_with_separators()
	JSON_COMMA,
	JSON_COLON,

	// These are only used internally as expectations of what comes next
	JSON_ITEM,
	JSON_KEY,
	JSON_VALUE,
} json_type;

typedef struct json_object {
	json_type type;
	struct json_object *parent;

	char *string;
	double number;

	struct json_object **array;
	struct json_object **keys;
	struct json_object **values;
	int length;

	int expect;
} json_object;

typedef struct json_pull {
	char *error;
	int line;

	int (*read)(struct json_pull *, char *buf, int n);
	void *source;
	char *buffer;
	int buffer_tail;
	int buffer_head;

	json_object *container;
	json_object *root;
} json_pull;

json_pull *json_begin_file(FILE *f);
json_pull *json_begin_string(char *s);

json_pull *json_begin(int (*read)(struct json_pull *, char *buffer, int n), void *source);
void json_end(json_pull *p);

typedef void (*json_separator_callback)(json_type type, json_pull *j, void *state);

json_object *json_read_tree(json_pull *j);
json_object *json_read(json_pull *j);
json_object *json_read_separators(json_pull *j, json_separator_callback cb, void *state);
void json_free(json_object *j);
void json_disconnect(json_object *j);

json_object *json_hash_get(json_object *o, const char *s);

char *json_stringify(json_object *o);
