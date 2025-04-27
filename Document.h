#pragma once

/*****************************************************************************
 * Document Class: A Singleton. Encapsulates all program data. 
 * Implements the Document-View design pattern, ensuring data centralization 
 * and accessibility.
 *****************************************************************************/
class Document {
private:
    static Document* instance; // Static instance pointer
    
    // Private constructor to prevent direct instantiation.
    Document();
 
    // Delete copy constructor and assignment operator to enforce Singleton behavior. 
    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;

public:    
    // Static method to get the singleton instance.   
    static Document* getInstance();
};



