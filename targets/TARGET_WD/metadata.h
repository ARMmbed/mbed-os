/*
 * metadata.h
 *
 * Created: 16.08.2017 15:05:12
 *  Author: Patrick.Frischmann
 */ 


#ifndef METADATA_H_
#define METADATA_H_

// Utility-Macros
#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

// Derived metadata settings
#define APPLICATION_NAME_STRING EXPAND_AND_QUOTE(APPLICATION_NAME)
#define APPLICATION_VERSION_STRING EXPAND_AND_QUOTE(APPLICATION_VERSION)

#endif /* METADATA_H_ */