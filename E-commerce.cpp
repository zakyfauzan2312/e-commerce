#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <ctime>

using namespace std;

// Struktur untuk Node di dalam Graph
struct GraphNode {
    int vertex;
    float weight;
    GraphNode* next;
};

// Struktur untuk menyimpan Graph
struct Graph {
    GraphNode* head[100];
    int vertices;
};

// Struktur untuk Produk
struct Product {
    string productType;
    int productID;
    char name[100];
    float price;
    int stock;
    char photo[255];
    char description[255];
};

// Struktur untuk Pengguna
struct User {
    int userID;
    char username[50];
    char password[50];
    char email[100];
    bool isAdmin;
};

// Node untuk Linked List Produk
struct ProductNode {
    Product data;
    ProductNode* next;
};

// Struktur untuk menyimpan data Produk
struct ProductList {
    ProductNode* head;
};

// Struktur untuk Pesanan
struct Order {
    int orderID;
    int userID;
    int destination;
    float totalPrice;
    char shippingService[20];
    char paymentMethod[20];
    char address[255];
    bool isShipped;
    bool isCancelled;
    bool cancelRequest;
};

// Node untuk Linked List Pesanan
struct OrderNode {
    Order data;
    OrderNode* next;
};

// Struktur untuk menyimpan data Pesanan
struct OrderList {
    OrderNode* head;
};

// Implementasi manual untuk struktur data stack
struct StackNode {
    Product data;
    StackNode* next;
};

struct Stack {
    StackNode* top;

    Stack() : top(nullptr) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void push(const Product& product) {
        StackNode* newNode = new StackNode();
        newNode->data = product;
        newNode->next = top;
        top = newNode;
    }

    Product pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        StackNode* temp = top;
        Product product = top->data;
        top = top->next;
        delete temp;
        return product;
    }
};

float calculateDistance(const Graph& graph, int source, int destination);

void addProduct(ProductList& list, Product product) {
    ProductNode* newNode = new ProductNode();
    newNode->data = product;
    newNode->next = list.head;
    list.head = newNode;
}

void displayProducts(ProductList& list) {
    ProductNode* current = list.head;
    if (current == nullptr) {
        cout << "No products available." << endl;
    } else {
        cout << "ID\tType\t\tName\t\tPrice\t\tStock\tPhoto\t\t\t\tDescription" << endl;
        cout << "---------------------------------------------------------------------------------------------------------------" << endl;
        while (current != nullptr) {
            cout << current->data.productID << " \t"
                 << current->data.productType << " \t"
                 << current->data.name << "\t"
                 << current->data.price << "\t\t"
                 << current->data.stock << "\t"
                 << current->data.photo << "\t"
                 << current->data.description << "\n";
            current = current->next;
        }
    }
}

int globalCounter = 0;

int generateUserID(int year, int month, int day, int counter) {
    return year * 1000000 + month * 10000 + day * 100 + counter;
}

void registerAccount() {
    ofstream outFile("account.txt", ios::app);
    if (!outFile.is_open()) {
        return;
    }

    User user;
    cout << "Enter email: ";
    cin >> user.email;
    cout << "Enter password: ";
    cin >> user.password;
    cout << "Is admin? (1 for Yes, 0 for No): ";
    cin >> user.isAdmin;

    int year = 2024;
    int month = 5;
    int day = 24;

    user.userID = generateUserID(year, month, day, ++globalCounter);

    strcpy(user.username, user.email);

    outFile << "UserID: " << user.userID << "\n"
            << "Username: " << user.username << "\n"
            << "Password: " << user.password << "\n"
            << "Email: " << user.email << "\n"
            << "IsAdmin: " << user.isAdmin << "\n"
            << "--------------------------" << "\n";

    outFile.close();
    cout << "Account registered successfully!" << endl;
}

bool login(User& loggedInUser) {
    ifstream inFile("account.txt");
    if (!inFile.is_open()) {
        return false;
    }

    char inputUsername[50], inputPassword[50];
    cout << "Enter email: ";
    cin >> inputUsername;
    cout << "Enter password: ";
    cin >> inputPassword;

    string line;
    while (getline(inFile, line)) {
        if (line.find("UserID: ") != string::npos) {
            int userID = stoi(line.substr(line.find(": ") + 2));
            string username, password, email;
            bool isAdmin;

            getline(inFile, line); // Username
            username = line.substr(line.find(": ") + 2);

            getline(inFile, line); // Password
            password = line.substr(line.find(": ") + 2);

            getline(inFile, line); // Email
            email = line.substr(line.find(": ") + 2);

            getline(inFile, line); // IsAdmin
            isAdmin = (line.substr(line.find(": ") + 2) == "1");

            getline(inFile, line);

            if (strcmp(inputUsername, username.c_str()) == 0 && strcmp(inputPassword, password.c_str()) == 0) {
                loggedInUser.userID = userID;
                strcpy(loggedInUser.username, username.c_str());
                strcpy(loggedInUser.password, password.c_str());
                strcpy(loggedInUser.email, email.c_str());
                loggedInUser.isAdmin = isAdmin;
                inFile.close();
                return true;
            }
        }
    }

    inFile.close();
    return false;
}

void saveProductsToFile(ProductList& list) {
    ofstream outFile("products.txt");
    if (!outFile.is_open()) {
        return;
    }
    
    ProductNode* current = list.head;
    while (current != nullptr) {
        outFile << "ProductType: " << current->data.productType << "\n"
                << "ProductID: " << current->data.productID << "\n"
                << "Name: " << current->data.name << "\n"
                << "Price: " << current->data.price << "\n"
                << "Stock: " << current->data.stock << "\n"
                << "Photo: " << current->data.photo << "\n"
                << "Description: " << current->data.description << "\n"
                << "=====================" << "\n";
        current = current->next;
    }
    
    outFile.close();
}

void loadProductsFromFile(ProductList& list) {
    ifstream inFile("products.txt");
    if (!inFile.is_open()) {
        return;
    }

    Product product;
    string line;
    while (getline(inFile, line)) {
        if (line.find("ProductType: ") != string::npos) {
            product.productType = line.substr(line.find(": ") + 2);
        }
        getline(inFile, line);
        if (line.find("ProductID: ") != string::npos) {
            product.productID = stoi(line.substr(line.find(": ") + 2));
        }
        getline(inFile, line);
        if (line.find("Name: ") != string::npos) {
            strcpy(product.name, line.substr(line.find(": ") + 2).c_str());
        }
        getline(inFile, line);
        if (line.find("Price: ") != string::npos) {
            product.price = stof(line.substr(line.find(": ") + 2));
        }
        getline(inFile, line);
        if (line.find("Stock: ") != string::npos) {
            product.stock = stoi(line.substr(line.find(": ") + 2));
        }
        getline(inFile, line);
        if (line.find("Photo: ") != string::npos) {
            strcpy(product.photo, line.substr(line.find(": ") + 2).c_str());
        }
        getline(inFile, line);
        if (line.find("Description: ") != string::npos) {
            strcpy(product.description, line.substr(line.find(": ") + 2).c_str());
        }
        getline(inFile, line);

        addProduct(list, product);
    }

    inFile.close();
}

int generateProductID(const string& productType) {
    static int counterElectronics = 0;
    static int counterClothing = 0;
    static int counterCosmetics = 0;
    static int counterOthers = 0;
    int prefix = 0;
    int suffix = 0;

    if (productType == "Electronics") {
        prefix = 1;
        suffix = ++counterElectronics;
    } else if (productType == "Clothing") {
        prefix = 2;
        suffix = ++counterClothing;
    } else if (productType == "Cosmetics") {
        prefix = 3;
        suffix = ++counterCosmetics;
    } else {
        prefix = 4;
        suffix = ++counterOthers;
    }

    if (suffix >= 100) {
        suffix = suffix % 100; 
    }

    return prefix * 100 + suffix;
}

void displayRatings() {
    ifstream ratingFile("ratings.txt");
    if (!ratingFile) {
        cout << "Tidak ada rating yang tersedia." << endl;
        return;
    }

    int productID, rating;
    cout << "ID Produk\t| Rating" << endl;
    cout << "-----------------------" << endl;

    while (ratingFile >> productID >> rating) {
        cout << productID << "\t\t| ";
        for (int i = 0; i < rating; i++) {
            cout << "*";
        }
        for (int i = rating; i < 5; i++) {
            cout << "^";
        }
        cout << endl;
    }
    ratingFile.close();
}


void saveOrdersToFile(OrderList& list) {
    ofstream outFile("orders.txt");
    if (!outFile.is_open()) {
        return;
    }
    OrderNode* current = list.head;
    while (current != nullptr) {
        outFile << "=== Order Entry ===" << endl;
        outFile << "Order ID: " << current->data.orderID << endl
                << "User ID: " << current->data.userID << endl
                << "Total Price: " << current->data.totalPrice << endl
                << "Shipping Service: " << current->data.shippingService << endl
                << "Payment Method: " << current->data.paymentMethod << endl
                << "Address: " << current->data.address << endl
                << "Is Shipped: " << current->data.isShipped << endl
                << "Is Cancelled: " << current->data.isCancelled << endl
                << "Cancel Request: " << current->data.cancelRequest << endl;
        outFile << "===================" << endl << endl;
        current = current->next;
    }
    outFile.close();
}

void sortProductsByPrice(ProductList& list, bool ascending = true) {
    if (list.head == nullptr || list.head->next == nullptr) {
        return;
    }

    bool swapped;
    ProductNode* ptr1;
    ProductNode* lptr = nullptr;

    do {
        swapped = false;
        ptr1 = list.head;

        while (ptr1->next != lptr) {
            bool condition = ascending ? (ptr1->data.price > ptr1->next->data.price) : (ptr1->data.price < ptr1->next->data.price);
            if (condition) {
                Product temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void loadOrdersFromFile(OrderList& list) {
    ifstream inFile("orders.txt");
    if (!inFile.is_open()) {
        return;
    }
    Order order;
    while (inFile >> order.orderID >> order.userID >> order.totalPrice) {
        inFile.ignore();
        inFile.getline(order.shippingService, 20);
        inFile.getline(order.paymentMethod, 30);
        inFile.getline(order.address, 255);
        inFile >> order.isShipped >> order.isCancelled >> order.cancelRequest;
        OrderNode* newNode = new OrderNode();
        newNode->data = order;
        newNode->next = list.head;
        list.head = newNode;
    }
    inFile.close();
}

void manageCancelRequests(OrderList& orderList, ProductList& productList) {
    OrderNode* current = orderList.head;
    while (current != nullptr) {
        if (current->data.cancelRequest && !current->data.isCancelled) {
            cout << "Order ID: " << current->data.orderID << " requested for cancellation." << endl;
            int approve;
            cout << "Approve cancellation? (1 for Yes, 0 for No): ";
            cin >> approve;
            if (approve == 1) {
                current->data.isCancelled = true;
                cout << "Order " << current->data.orderID << " has been cancelled." << endl;
                saveOrdersToFile(orderList);
                saveProductsToFile(productList);
            } else {
                current->data.cancelRequest = false;
                cout << "Cancellation request for order " << current->data.orderID << " has been denied." << endl;
                saveOrdersToFile(orderList);
            }
        }
        current = current->next;
    }
}

void adminMenu(ProductList& productList, OrderList& orderList) {
    int choice;
    while (true) {
        cout << "-------------------" << endl;
        cout << "Admin Menu:" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. View Products" << endl;
        cout << "3. Sort Products by Price" << endl;
        cout << "4. View Ratings" << endl;
        cout << "5. View Orders" << endl;
        cout << "6. Manage Cancel Req" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            Product product;
            int productTypeChoice;
            cout << "Select product type:" << endl;
            cout << "1. Electronics" << endl;
            cout << "2. Clothing" << endl;
            cout << "3. Cosmetics" << endl;
            cout << "4. Others" << endl;
            cout << "Enter your choice: ";
            cin >> productTypeChoice;

            switch (productTypeChoice) {
                case 1:
                    product.productType = "Electronics";
                    break;
                case 2:
                    product.productType = "Clothing";
                    break;
                case 3:
                    product.productType = "Cosmetics";
                    break;
                case 4:
                    product.productType = "Others";
                    break;
                default:
                    cout << "Invalid choice. Product not added." << endl;
                    continue;
            }

            product.productID = generateProductID(product.productType);
            cout << "Enter product name: ";
            cin.ignore();
            cin.getline(product.name, 100);
            cout << "Enter product price: ";
            cin >> product.price;
            cout << "Enter product stock: ";
            cin >> product.stock;
            cout << "Enter product photo (URL): ";
            cin.ignore();
            cin.getline(product.photo, 255);
            cout << "Enter product description: ";
            cin.getline(product.description, 255);
            addProduct(productList, product);
            saveProductsToFile(productList);
            cout << "Product added successfully!" << endl;
        } else if (choice == 2) {
            // menampilkan semua produk
            displayProducts(productList);
        } else if (choice == 3) {
            // mengurutkan produk berdasarkan harga
            int sortChoice;
            cout << "Sort by price:" << endl;
            cout << "1. From cheapest to most expensive" << endl;
            cout << "2. From most expensive to cheapest" << endl;
            cout << "Enter your choice: ";
            cin >> sortChoice;
            bool ascending = (sortChoice == 1);
            sortProductsByPrice(productList, ascending);
            cout << "Products sorted by price successfully!" << endl;
            displayProducts(productList);
        } else if (choice == 4) {
            //menampilkan rating produk
            displayRatings();
        } else if (choice == 5) {
            //menampilkan detail setiap pesanan 
            OrderNode* current = orderList.head;
            if (current == nullptr) {
                cout << "No orders available." << endl;
            } else {
                cout << "OrderID\tUserID\tTotalPrice\tShippingService\tPaymentMethod\tAddress\t\tStatus" << endl;
                cout << "---------------------------------------------------------------------------------------------------------------" << endl;
                while (current != nullptr) {
                    cout << current->data.orderID << "\t"
                         << current->data.userID << "\t"
                         << current->data.totalPrice << "\t\t"
                         << current->data.shippingService << "\t\t"
                         << current->data.paymentMethod << "\t"
                         << current->data.address << "\t"
                         << (current->data.isShipped ? "Shipped" : "Not Shipped") << "\t"
                         << (current->data.isCancelled ? "Cancelled" : "Active") << "\t"
                         << (current->data.cancelRequest ? "Cancel Requested" : "No Cancel Request") << endl;
                    current = current->next;
                }
            }
        } else if (choice == 6) {
            // mengelola permintaan pembatalan pesanan
            manageCancelRequests(orderList, productList);
        } else if (choice == 7) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

void addToCart(Stack& cart, ProductList& productList, int productID, int quantity) {
    ProductNode* current = productList.head;// disini untuk memulai iterasi dari awal produk
    while (current != nullptr) {// digunakan untuk megiterasi malalui linkedlist
        if (current->data.productID == productID && current->data.stock >= quantity) {
            //memeriksa produk apakah cocok dengan produk yang dicari
            //memeriksa juga apakah stoknya lebih dari atau sama dengan jumlah yang dicari
            Product product = current->data;// jika kondisi diatas terpenuhi,
            // maka produk yang dicari akan disimpan dalam variabel product
            product.stock = quantity;
            //stok produk disalin dan diatur kejumlah yng diminta
            cart.push(product);// menambahkan produk ke dalam keranjang mengunnakan metode push
            cout << "Product added to cart!" << endl;
            return;
        }
        current = current->next;
        // jika kondisi diatas tidak terpenuhi, pointer current pindah ke node selanjutnya
    }
    cout << "Product not found or insufficient stock!" << endl;
}

string cityNames[] = {"Surabaya", "Bandung", "Jakarta", "Malang", "Sidoarjo", "Jogjakarta", "Bali"};

// fungsi untuk menambahkan edge kedalam graf
void addEdge(Graph& graph, int u, int v, float weight) {
    // mmbuat node baru  untuk vrtex v
    GraphNode* newNode = new (nothrow) GraphNode();
    if (!newNode) {
        cerr << "Memory allocation failed for newNode in addEdge" << endl;
        exit(1);
    }
    newNode->vertex = v;
    // mengatur vertex dari node baru ke v bahwa node ini terhubung dengan v
    newNode->weight = weight;
    newNode->next = graph.head[u];
    //node baru menunjukkan kenode sebelimnya dari dftr tsb
    graph.head[u] = newNode;

    // mmbuat node baru untuk vertex u(graf tak berarah)
    newNode = new (nothrow) GraphNode();
    if (!newNode) {
        cerr << "Memory allocation failed for newNode in addEdge" << endl;
        exit(1);
    }
    newNode->vertex = u;
    newNode->weight = weight;
    newNode->next = graph.head[v];
    graph.head[v] = newNode;
}
// fungsi untuk menginisialisasi graf dengan 7 vertex dan menmbahkan
// edge-edgenya masing-masing dengan bobot trentu yg menghubungkan kota-kota
void initializeDeliveryGraph(Graph& graph) {
    graph.vertices = 7;// graf memiliki 7 vertex, yang mewakili 7 kota
    for (int i = 0; i < graph.vertices; ++i) { 
        graph.head[i] = nullptr;
        // menginisialisasi daftar adjacencylist menjadi nullptr
        // karena belum ada edge yang ditambahkan                        
    }
    // akan menambahkan edge yang akan menghubungkan 
    // kota-kota yang ada di dalam graph beserta bobot jaraknya
    addEdge(graph, 0, 1, 750.0);  // Surabaya ke Bandung
    addEdge(graph, 0, 2, 800.0);  // Surabaya ke Jakarta
    addEdge(graph, 0, 3, 90.0);   // Surabaya ke Malang
    addEdge(graph, 0, 4, 25.0);   // Surabaya ke Sidoarjo
    addEdge(graph, 0, 5, 330.0);  // Surabaya ke Jogjakarta
    addEdge(graph, 0, 6, 400.0);  // Surabaya ke Bali
    addEdge(graph, 1, 2, 150.0);  // Bandung ke Jakarta
    addEdge(graph, 1, 3, 700.0);  // Bandung ke Malang
    addEdge(graph, 1, 4, 725.0);  // Bandung ke Sidoarjo
    addEdge(graph, 1, 5, 450.0);  // Bandung ke Jogjakarta
    addEdge(graph, 1, 6, 950.0);  // Bandung ke Bali
    addEdge(graph, 2, 3, 790.0);  // Jakarta ke Malang
    addEdge(graph, 2, 4, 805.0);  // Jakarta ke Sidoarjo
    addEdge(graph, 2, 5, 540.0);  // Jakarta ke Jogjakarta
    addEdge(graph, 2, 6, 970.0);  // Jakarta ke Bali
    addEdge(graph, 3, 4, 95.0);   // Malang ke Sidoarjo
    addEdge(graph, 3, 5, 340.0);  // Malang ke Jogjakarta
    addEdge(graph, 3, 6, 480.0);  // Malang ke Bali
    addEdge(graph, 4, 5, 330.0);  // Sidoarjo ke Jogjakarta
    addEdge(graph, 4, 6, 420.0);  // Sidoarjo ke Bali
    addEdge(graph, 5, 6, 530.0);  // Jogjakarta ke Bali
}

void chooseDeliveryDestination(int& destination, const Graph& graph) {
    cout << "Choose Delivery Destination:" << endl;
    for (int i = 0; i < graph.vertices; ++i) {
        // loop digunaakan untuk mengiterasi vertex kota dari graf
        cout << i + 1 << ". " << cityNames[i] << endl;
        // menampilkan nama kota dari graf
    }
    cout << "Enter your choice: ";
    cin >> destination;
    destination--;

    if (destination < 0 || destination >= graph.vertices) {
        // jika pilihan tidak sesuai dengan jumlah vertex graf
        cout << "Invalid choice. Please try again." << endl;
        chooseDeliveryDestination(destination, graph);
        //memanggil fungsi chooseDeliveryDestination kembali
    }
}

void chooseShippingService(char shippingService[]) {
    int choice;
    cout << "Choose Shipping Service:" << endl;
    cout << "1. Regular" << endl;
    cout << "2. Express" << endl;
    cout << "3. Same Day" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            strcpy(shippingService, "Regular");
            break;
        case 2:
            strcpy(shippingService, "Express");
            break;
        case 3:
            strcpy(shippingService, "Same Day");
            break;
        default:
            cout << "Invalid choice. Defaulting to Regular." << endl;
            strcpy(shippingService, "Regular");
            break;
    }
}

void choosePaymentMethod(char paymentMethod[]) {
    int choice;
    cout << "Choose Payment Method:" << endl;
    cout << "1. Credit Card" << endl;
    cout << "2. Bank Transfer" << endl;
    cout << "3. E-Wallet" << endl;
    cout << "4. Cash on Delivery (COD)" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            strcpy(paymentMethod, "Credit Card");
            break;
        case 2:
            strcpy(paymentMethod, "Bank Transfer");
            break;
        case 3:
            strcpy(paymentMethod, "E-Wallet");
            break;
        case 4:
            strcpy(paymentMethod, "Cash on Delivery (COD)");
            break;
        default:
            strcpy(paymentMethod, "Credit Card");
            break;
    }
}

void printReceipt(User& user, int orderID, Stack& cart, const char shippingService[], const char paymentMethod[], float totalPrice, const char address[], float shippingCost, bool isCancelled, bool cancelRequest, int source, int destination, float distance) {
    ofstream receiptFile("e-bill.txt");
    if (!receiptFile.is_open()) {
        cout << "Failed to open receipt file." << endl;
        return;
    }

    receiptFile << "=== Order Receipt ===" << endl;
    receiptFile << "Order ID: " << orderID << endl;
    receiptFile << "Username: " << user.username << endl;
    receiptFile << "Email: " << user.email << endl;
    receiptFile << "Address: " << address << endl;
    receiptFile << "Store Location for Shipment: " << cityNames[source] << endl;
    receiptFile << "Delivery Destination: " << cityNames[destination] << endl;
    receiptFile << "Distance: " << distance << " km" << endl;
    receiptFile << "Products:" << endl;

    Stack tempCart;
    while (!cart.isEmpty()) {
        Product product = cart.pop();
        receiptFile << "  ID: " << product.productID << endl;
        receiptFile << "  Name: " << product.name << endl;
        receiptFile << "  Quantity: " << product.stock << endl;
        receiptFile << "  Price: " << product.price << endl;
        receiptFile << "  Total: " << product.price * product.stock << endl;
        receiptFile << "----------------------------------------" << endl;
        tempCart.push(product);
    }

    while (!tempCart.isEmpty()) {
        cart.push(tempCart.pop());
    }

    receiptFile << "Shipping Service: " << shippingService << endl;
    receiptFile << "Payment Method: " << paymentMethod << endl;
    if (totalPrice > 30000) {
        receiptFile << "Shipping Cost: Rp" << shippingCost << " Free (for orders above 30000)" << endl;
    } else {
        receiptFile << "Shipping Cost: Rp" << shippingCost << endl;
    }

    float totalAmmount = totalPrice + shippingCost; 
    receiptFile << "Total Price: Rp" << totalAmmount << endl;

    if (cancelRequest) {
        receiptFile << "Cancellation Request: " << (isCancelled ? "Accepted" : "Rejected") << endl;
    }

    receiptFile << "=====================" << endl;
    receiptFile.close();
    cout << "E-bill has been printed to e-bill.txt" << endl;
}


void requestCancelOrder(User& user, OrderList& orderList) {
    int orderID;
    cout << "Enter the Order ID to request cancellation: ";
    cin >> orderID;

    OrderNode* current = orderList.head;
    while (current != nullptr) {
        if (current->data.orderID == orderID && current->data.userID == user.userID) {
            if (current->data.isShipped) {
                cout << "Order already shipped. Cannot be cancelled." << endl;
                return;
            }
            if (current->data.isCancelled) {
                cout << "Order already cancelled." << endl;
                return;
            }
            current->data.cancelRequest = true;
            cout << "Cancellation request for order " << orderID << " has been submitted." << endl;
            saveOrdersToFile(orderList);
            return;
        }
        current = current->next;
    }
    cout << "Order not found." << endl;
}

void viewCart(Stack& cart) {
    if (cart.isEmpty()) {
        cout << "Cart is empty!" << endl;
        return;
    }

    float totalPrice = 0;
    Stack tempCart;
    while (!cart.isEmpty()) {
        Product product = cart.pop();
        totalPrice += product.price * product.stock;
        cout << "ID: " << product.productID << ", Name: " << product.name
             << ", Price: " << product.price << ", Quantity: " << product.stock
             << ", Photo: " << product.photo << ", Description: " << product.description << endl;
        tempCart.push(product);
    }

    while (!tempCart.isEmpty()) {
        cart.push(tempCart.pop());
    }

    cout << "Total Price: " << totalPrice << endl;
}

void removeFromCart(Stack& cart, int productID, ProductList& productList) {
    Stack tempCart;
    bool found = false;
    while (!cart.isEmpty()) {
        Product product = cart.pop();
        if (product.productID == productID) {
            found = true;
            break;
        } else {
            tempCart.push(product);
        }
    }
    
    while (!tempCart.isEmpty()) {
        cart.push(tempCart.pop());
    }

    if (found) {
        cout << "Product removed from cart!" << endl;
    } else {
        cout << "Product not found in cart!" << endl;
    }
}

void checkout(Stack& cart, User& user, ProductList& productList, OrderList& orderList, const Graph& graph) {
    if (cart.isEmpty()) {
        cout << "Cart is empty!" << endl;
        return;
    }

    float totalPrice = 0;
    Stack tempCart;
    while (!cart.isEmpty()) {
        Product product = cart.pop();
        totalPrice += product.price * product.stock;
        cout << "ID: " << product.productID << ", Name: " << product.name
             << ", Price: " << product.price << ", Quantity: " << product.stock
             << ", Photo: " << product.photo << ", Description: " << product.description << endl;
        tempCart.push(product);
    }

    while (!tempCart.isEmpty()) {
        cart.push(tempCart.pop());
    }

    cout << "Total Price: " << totalPrice << endl;

    char shippingService[20];
    chooseShippingService(shippingService);

    char paymentMethod[20];
    choosePaymentMethod(paymentMethod);

    int source;
    cout << "Choose store location for shipment:" << endl;
    for (int i = 0; i < graph.vertices; ++i) {
        cout << i + 1 << ". " << cityNames[i] << endl;
    }
    cout << "Enter your choice: ";
    cin >> source;
    source--;

    int destination;
    chooseDeliveryDestination(destination, graph);

    float distance = calculateDistance(graph, source, destination);
    if (distance < 0) {
        cout << "Invalid route from store to destination." << endl;
        return;
    }

    float shippingCost = 0;
    if (totalPrice <= 30000) {
        shippingCost = ((distance + 9) / 50) * 22000;
    }

    char address[255];
    cout << "Enter shipping address: ";
    cin.ignore();
    cin.getline(address, 255);

    Stack checkoutCart;
    while (!cart.isEmpty()) {
        Product product = cart.pop();
        ProductNode* current = productList.head;
        while (current != nullptr) {
            if (current->data.productID == product.productID) {
                current->data.stock -= product.stock;
                break;
            }
            current = current->next;
        }
        checkoutCart.push(product);
    }
    saveProductsToFile(productList);

    Order order;
    order.orderID = time(nullptr);
    order.userID = user.userID;
    order.totalPrice = totalPrice;
    order.destination = destination;
    strcpy(order.shippingService, shippingService);
    strcpy(order.paymentMethod, paymentMethod);
    strcpy(order.address, address);
    order.isShipped = false;
    order.isCancelled = false;
    order.cancelRequest = false;

    OrderNode* newNode = new (nothrow) OrderNode();
    if (!newNode) {
        cerr << "Memory allocation failed for newNode in checkout" << endl;
        return;
    }
    newNode->data = order;
    newNode->next = orderList.head;
    orderList.head = newNode;
    saveOrdersToFile(orderList);

    printReceipt(user, order.orderID, checkoutCart, shippingService, paymentMethod, totalPrice, address, shippingCost, order.isCancelled, order.cancelRequest, source, destination, distance);

    cout << "Order placed successfully!" << endl;
}

float calculateDistance(const Graph& graph, int source, int destination) {
    GraphNode* current = graph.head[source];
    while (current != nullptr) {
        if (current->vertex == destination) {
            return current->weight;
        }
        current = current->next;
    }
    return -1;
}

void rateProducts(ProductList& productList, int productID, int rating) {
    ProductNode* current = productList.head;
    while (current != nullptr) {
        if (current->data.productID == productID) {
            ofstream ratingFile("ratings.txt", ios::app);
            if (ratingFile.is_open()) {
                ratingFile << "Product: " << productID << "|Rating: " << rating << endl;
                ratingFile.close();
            } else {
                cout << "Unable to open ratings file." << endl;
                return;
            }

            cout << "You rated " << current->data.name << " with ";
            for (int j = 0; j < rating; j++) {
                cout << "*";
            }
            for (int j = rating; j < 5; j++) {
                cout << "^";
            }
            cout << endl;
            return;
        }
        current = current->next;
    }
    cout << "Product not found!" << endl;
}

struct TreeNode {
    string category;
    TreeNode* left;
    TreeNode* right;
};

// menambahkan kategori baru kedalam tree biner berdasarkan 
//aturan tertentu dimana kategori yang lebih kecil ditempatkan sebelah kiri
TreeNode* addCategory(TreeNode* root, const string& category) {
    if (root == nullptr) { // jika pohon kosong 
        root = new TreeNode(); // membuat node baru
        root->category = category;//menetapkan kategori tsb kenode
        root->left = nullptr;//inisialisasi node sebelah kiri
        root->right = nullptr;
    } else if (category < root->category) {
        root->left = addCategory(root->left, category);
        // jika kategori baru lebih kecil dari kategori root,
        //melkukan rekursi utk menambahkan kategori baru kesub-pohon kiri
    } else {
        root->right = addCategory(root->right, category);
        // jika kategori baru lebih besar dari kategori root,
        // melkukan rekursi utk menambahkan kategori baru kesub-pohon kanan
    }
    return root;
}

// menampilkan kategori yang ada di pohon biner menggunakan mtode traversal inorder
// untuk transversal inorder sendiriakan mengjungi node dengan urutan dari kiri
// ke node saat ini kemudian ke kanan
void displayCategories(TreeNode* root) {
    if (root != nullptr) {// jika pohon tidak kosong
        displayCategories(root->left);
        //karena traversal inorder, kita akan menampilkan kategori kiri dulu
        //mengunjungi semua node disubtree kiri
        cout << root->category << endl;
        // mencetak kategori node saat ini
        displayCategories(root->right);
    }
}

void displayDeliveryGraph(Graph& graph) {
    cout << "Delivery Map" << endl;
    for (int i = 0; i < graph.vertices; ++i) {
        GraphNode* temp = graph.head[i];
        cout << "-----------------------------------" << endl;
        cout << "From Store at " << cityNames[i] << ": " << endl;
        while (temp) {
            cout << "to Delivery Destination " << cityNames[temp->vertex] << " (" << temp->weight << " km) " << endl;
            temp = temp->next;
        }
    }
}

struct HashTable {
    Product data[100];
    bool occupied[100] = { false };

    int hash(int key) {
        return key % 100;
    }

// collision handling dengan linear probing
    void insert(Product product) {
        int index = hash(product.productID);// menghitung indeks awal mengunakan fungsi hash
        while (occupied[index]) {// jika indeks sudah terisi
            index = (index + 1) % 100;// mengunakan probing linear untuk menemukan indeks
            //berikutnya yang kosong
        }
        data[index] = product;// mrnyimpan produk pada indeks yg ditemukan
        occupied[index] = true;// menandai indeks tersebut telah terisi
    }
// searching produk berdasarkan productID menggunakan linear probing
    Product* search(int productID) {
        int index = hash(productID); // menghitung indeks awal mengunakan fungsi hash
        int start = index; // menyimpan indeks awal
        while (occupied[index] && data[index].productID != productID) {
            //selama indeks terisi dan produk tidak ditemukan
            index = (index + 1) % 100;// mengunkan probing linear untuk menemukan indeks berikutnya
            if (index == start) { // jika kembali ke indeks awal,
                return nullptr; // artinya tabel penuh dan produk tidak ditemukan
            }
        }
        if (occupied[index]) {// jika indeks terisi dan produk ditemukan
            return &data[index];// mengembalikan alamat ke produk
        } else {
            return nullptr; // jika produk tidak ditemukn
        }
    }
};

void userMenu(ProductList& productList, User& user, OrderList& orderList, const Graph& graph) {
    Stack cart;
    int choice;
    while (true) {
        cout << "-------------------" << endl;
        cout << "User Menu:" << endl;
        cout << "1. View Products" << endl;
        cout << "2. Add to Cart" << endl;
        cout << "3. View Cart" << endl;
        cout << "4. Remove from Cart" << endl;
        cout << "5. Checkout" << endl;
        cout << "6. Rate Products" << endl;
        cout << "7. Req Order Cancel" << endl;
        cout << "8. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            displayProducts(productList);
        } else if (choice == 2) {
            int productID, quantity;
            cout << "Enter product ID: ";
            cin >> productID;
            cout << "Enter quantity: ";
            cin >> quantity;
            addToCart(cart, productList, productID, quantity);
        } else if (choice == 3) {
            viewCart(cart);
        } else if (choice == 4) {
            int productID;
            cout << "Enter product ID to remove: ";
            cin >> productID;
            removeFromCart(cart, productID, productList);
        } else if (choice == 5) {
            checkout(cart, user, productList, orderList, graph);
        } else if (choice == 6) {
            int productID, rating;
            cout << "Enter product ID: ";
            cin >> productID;
            cout << "Enter rating (1-5): ";
            cin >> rating;
            rateProducts(productList, productID, rating);
        } else if (choice == 7) {
            requestCancelOrder(user, orderList);
        } else if (choice == 8) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

int main() {
    ProductList productList;
    productList.head = nullptr;

    OrderList orderList;
    orderList.head = nullptr;

    loadProductsFromFile(productList);
    loadOrdersFromFile(orderList);

    Graph deliveryGraph;
    initializeDeliveryGraph(deliveryGraph);

    TreeNode* rootCategory = nullptr;
    rootCategory = addCategory(rootCategory, "All Categories");
    rootCategory = addCategory(rootCategory, "Electronics");
    rootCategory = addCategory(rootCategory, "Clothing");
    rootCategory = addCategory(rootCategory, "Cosmetics");
    rootCategory = addCategory(rootCategory, "Others");

    HashTable productHashTable;
    ProductNode* current = productList.head;
    while (current != nullptr) {
        productHashTable.insert(current->data);
        current = current->next;
    }

    while (true) {
        int choice;
        cout << "-------------------" << endl;
        cout << "Main Menu:" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. View Categories" << endl;
        cout << "4. View Delivery Graph" << endl;
        cout << "5. Search Product by ID" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            registerAccount();
        } else if (choice == 2) {
            User loggedInUser;
            if (login(loggedInUser)) {
                if (loggedInUser.isAdmin) {
                    adminMenu(productList, orderList);
                } else {
                    userMenu(productList, loggedInUser, orderList, deliveryGraph);
                }
            } else {
                cout << "Login failed. Invalid email or password." << endl;
            }
        } else if (choice == 3) {
            displayCategories(rootCategory);
        } else if (choice == 4) {
            displayDeliveryGraph(deliveryGraph);
        } else if (choice == 5) {
            int productID;
            cout << "Enter product ID to search: ";
            cin >> productID;
            Product* product = productHashTable.search(productID);
            if (product) {
                cout << "Product found: " << product->name << ", Price: " << product->price << ", Stock: " << product->stock << endl;
            } else {
                cout << "Product not found!" << endl;
            }
        } else if (choice == 6) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}