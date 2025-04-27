#include "Document.h"

/*****************************************************************************
 * Document Class: Implementation for managing program data and simulation
 * results. Ensures a single instance of the data management system.
 *****************************************************************************/

 //------------------------------------------------------------------------
 // Initialize the static instance pointer.
 //------------------------------------------------------------------------
Document* Document::instance = nullptr;

//------------------------------------------------------------------------
// Private constructor to prevent direct instantiation.
//------------------------------------------------------------------------
Document::Document() {}

//------------------------------------------------------------------------
// Static method to get the singleton instance.
//------------------------------------------------------------------------
Document* Document::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Document();
    }
    return instance;
}