/*  _______________________________________________________________________

    DAKOTA: Design Analysis Kit for Optimization and Terascale Applications
    Copyright (c) 2010, Sandia National Laboratories.
    This software is distributed under the GNU Lesser General Public License.
    For more information, see the README file in the top Dakota directory.
    _______________________________________________________________________ */

//- Class:        ProblemDescDB
//- Description:  Base class for Dakota problem description database
//- Owner:        Mike Eldred
//- Version: $Id: ProblemDescDB.hpp 6798 2010-05-24 20:29:12Z wjbohnh $

#ifndef PROBLEM_DESC_DB_H
#define PROBLEM_DESC_DB_H

#include "dakota_system_defs.hpp"
#include "dakota_data_types.hpp"
#include "DataStrategy.hpp"
#include "DataMethod.hpp"
#include "DataModel.hpp"
#include "DataVariables.hpp"
#include "DataInterface.hpp"
#include "DataResponses.hpp"

namespace Dakota {

class ParallelLibrary;
class CommandLineHandler;


/// The database containing information parsed from the DAKOTA input file.

/** The ProblemDescDB class is a database for DAKOTA input file data
    that is populated by a parser defined in a derived class.  When
    the parser reads a complete keyword, it populates a data class
    object (DataStrategy, DataMethod, DataVariables, DataInterface, or
    DataResponses) and, for all cases except strategy, appends the
    object to a linked list (dataMethodList, dataVariablesList,
    dataInterfaceList, or dataResponsesList).  No strategy linked list
    is used since only one strategy specification is allowed. */

class ProblemDescDB
{
  //
  //- Heading: Friends
  //

  /// Model requires access to get_variables() and get_response()
  friend class Model;
  /// SingleModel requires access to get_interface()
  friend class SingleModel;
  /// HierarchSurrModel requires access to get_model()
  friend class HierarchSurrModel;
  /// DataFitSurrModel requires access to get_iterator() and get_model()
  friend class DataFitSurrModel;
  /// NestedModel requires access to get_interface(), get_response(),
  /// get_iterator(), and get_model()
  friend class NestedModel;

  /// Strategy requires access to get_iterator()
  friend class Strategy;
  /// SingleMethodStrategy requires access to get_model()
  friend class SingleMethodStrategy;
  /// HybridStrategy requires access to get_model()
  friend class HybridStrategy;
 /// SequentialStrategy requires access to get_iterator()
  friend class SequentialHybridStrategy;
  /// ConcurrentStrategy requires access to get_model()
  friend class ConcurrentStrategy;

  /// SurrBasedLocalMinimizer requires access to get_iterator()
  friend class SurrBasedLocalMinimizer;
  /// SurrBasedGlobalMinimizer requires access to get_iterator()
  friend class SurrBasedGlobalMinimizer;

public:

  //
  //- Heading: Constructors, destructor, assignment operator
  //

  /// default constructor
  ProblemDescDB();
  /// standard constructor
  ProblemDescDB(ParallelLibrary& parallel_lib);
  /// copy constructor
  ProblemDescDB(const ProblemDescDB& db);

  /// destructor
  ~ProblemDescDB();

  /// assignment operator
  ProblemDescDB operator=(const ProblemDescDB& db);

  //
  //- Heading: Member methods
  //

  /// invokes manage_inputs(const char*, ...) using the dakota input filename
  /// passed with the "-input" option on the DAKOTA command line.  This is the
  /// normal API employed in main.cpp.
  void manage_inputs(CommandLineHandler& cmd_line_handler);
  /// invokes parse_inputs() to populate the problem description database and
  /// execute any callback function, broadcast() to propagate DB data to all
  /// processors, and post_process() to construct default variables/response
  /// vectors.  This is an alternate API used by the file parsing mode in
  /// library_mode.cpp.
  void manage_inputs(const char* dakota_input_file,
		     const char* parser_options = NULL,
		     bool echo_input = true,
		     void(*callback)(void*) = NULL, void* callback_data = NULL);
  /// parses the input file and populates the problem description database.
  /// This function reads from the dakota input filename passed in and
  /// allows subsequent modifications to be done by a callback function.
  /// This API is used by the mixed mode option in library_mode.cpp since it
  /// allows broadcast() and post_process() to be deferred until all inputs
  /// have been provided.
  void parse_inputs(const char* dakota_input_file,
		    const char* parser_options = NULL,
		    bool echo_input = true,
		    void(*callback)(void*) = NULL, void* callback_data = NULL);

  /// verifies that there is at least one of each of the required
  /// keywords in the dakota input file.  Used by parse_inputs().
  void check_input(const char* dakota_input_file = NULL, bool echo_input = true);
  /// invokes send_db_buffer() and receive_db_buffer() to broadcast DB
  /// data across the processor allocation.  Used by manage_inputs().
  void broadcast();
  /// post-processes the (minimal) input specification to assign default
  /// variables/responses specification arrays.  Used by manage_inputs().
  void post_process();
  /// Locks the database in order to prevent data access when the list nodes
  /// may not be set properly.  Unlocked by a set nodes operation.
  void lock();
  /// Explicitly unlocks the database.  Use with care.
  void unlock();

  /// set dataMethodIter based on a method identifier string to activate a
  /// particular method specification in dataMethodList and use pointers from
  /// this method specification to set all other list iterators.
  void set_db_list_nodes(const String& method_tag);
  /// set dataMethodIter based on an index within dataMethodList to activate a
  /// particular method specification and use pointers from this method
  /// specification to set all other list iterators.
  void set_db_list_nodes(const size_t& method_index);
  /// For a (default) strategy lacking a method pointer, this function is used
  /// to determine which of several potential method specifications corresponds
  /// to the top method and then sets the list nodes accordingly.
  void resolve_top_method();

  /// set dataMethodIter based on a method identifier string to activate a
  /// particular method specification (only).
  void set_db_method_node(const String& method_tag);
  /// set dataMethodIter based on an index within dataMethodList to activate a
  /// particular method specification (only).
  void set_db_method_node(const size_t& method_index);
  /// return the index of the active node in dataMethodList
  size_t get_db_method_node(); // restoration usage: return by value

  /// set the model list iterators (dataModelIter, dataVariablesIter,
  /// dataInterfaceIter, and dataResponsesIter) based on the model
  /// identifier string
  void set_db_model_nodes(const String& model_tag);
  /// set the model list iterators (dataModelIter, dataVariablesIter,
  /// dataInterfaceIter, and dataResponsesIter) based on an index
  /// within dataModelList
  void set_db_model_nodes(const size_t& model_index);
  /// return the index of the active node in dataModelList
  size_t get_db_model_node(); // restoration usage: return by value

  /// set dataVariablesIter based on the variables identifier string
  void set_db_variables_node(const String& variables_tag);
  /// set dataInterfaceIter based on the interface identifier string
  void set_db_interface_node(const String& interface_tag);
  /// set dataResponsesIter based on the responses identifier string
  void set_db_responses_node(const String& responses_tag);

  //
  //- Heading: Set/Inquire functions
  //

  /// return the parallelLib reference
  ParallelLibrary& parallel_library() const;

  /// return a list of all Iterator objects that have been instantiated
  IteratorList& iterator_list();
  /// return a list of all Model objects that have been instantiated
  ModelList& model_list();
  /// return a list of all Variables objects that have been instantiated
  VariablesList& variables_list();
  /// return a list of all Interface objects that have been instantiated
  InterfaceList& interface_list();
  /// return a list of all Response objects that have been instantiated
  ResponseList& response_list();

  // These functions get values out of the database.  A value is found by its
  // entry_name. Need a HashTable or other container with an efficient lookup
  // function here.

  /// get a RealVector out of the database based on an identifier string
  const RealVector& get_rv(const String& entry_name) const;
  /// get an IntVector out of the database based on an identifier string
  const IntVector& get_iv(const String& entry_name) const;
  /// get an SizetArray out of the database based on an identifier string
  const SizetArray& get_sza(const String& entry_name) const;
  /// get an UShortArray out of the database based on an identifier string
  const UShortArray& get_usa(const String& entry_name) const;
  /// get a RealSymMatrix out of the database based on an identifier string
  const RealSymMatrix& get_rsm(const String& entry_name) const;
  /// get a RealVectorArray out of the database based on an identifier string
  const RealVectorArray& get_rva(const String& entry_name) const;
  /// get an IntVectorArray out of the database based on an identifier string
  const IntVectorArray& get_iva(const String& entry_name) const;
  /// get an IntSet out of the database based on an identifier string
  const IntSet& get_is(const String& entry_name) const;
  /// get an IntSetArray out of the database based on an identifier string
  const IntSetArray& get_isa(const String& entry_name) const;
  /// get a RealSetArray out of the database based on an identifier string
  const RealSetArray& get_rsa(const String& entry_name) const;
  /// get an IntRealMapArray out of the database based on an identifier string
  const IntRealMapArray& get_irma(const String& entry_name) const;
  /// get a RealRealMapArray out of the database based on an identifier string
  const RealRealMapArray& get_rrma(const String& entry_name) const;
  /// get a StringArray out of the database based on an identifier string
  const StringArray& get_sa(const String& entry_name) const;
  /// get a String2DArray out of the database based on an identifier string
  const String2DArray& get_s2a(const String& entry_name) const;
  /// get a String out of the database based on an identifier string
  const String& get_string(const String& entry_name) const;
  /// get a Real out of the database based on an identifier string
  const Real& get_real(const String& entry_name) const;
  /// get an int out of the database based on an identifier string
  int get_int(const String& entry_name) const;
  /// get a short out of the database based on an identifier string
  short get_short(const String& entry_name) const;
  /// get an unsigned short out of the database based on an identifier string
  unsigned short get_ushort(const String& entry_name) const;
  /// get a size_t out of the database based on an identifier string
  size_t get_sizet(const String& entry_name) const;
  /// get a bool out of the database based on an identifier string
  bool get_bool(const String& entry_name) const;
  /// for getting a void**, e.g., &dlLib
  void** get_voidss(const String& entry_name) const;

  // These functions support a library mode with external parsing.  Rather
  // than using manage_inputs() to parse an input file, Data objects
  // populated elsewhere can be inserted into the Data object lists.

  /// set the DataStrategy object
  void insert_node(const DataStrategy& data_strategy);
  /// add a DataMethod object to the dataMethodList
  void insert_node(const DataMethod& data_method);
  /// add a DataModel object to the dataModelList
  void insert_node(const DataModel& data_model);
  /// add a DataVariables object to the dataVariablesList
  void insert_node(DataVariables& data_variables);
  /// add a DataInterface object to the dataInterfaceList
  void insert_node(const DataInterface& data_interface);
  /// add a DataResponses object to the dataResponsesList
  void insert_node(const DataResponses& data_responses);

  // These functions support a mixed library mode: DAKOTA input file parsing is
  // combined with some external updates, particularly to vector inputs that
  // may be large and inconvenient to include in the DAKOTA input.

  /// set a RealVector within the database based on an identifier string
  void set(const String& entry_name, const RealVector& rv);
  /// set an IntVector within the database based on an identifier string
  void set(const String& entry_name, const IntVector& iv);
  /// set a RealMatrix within the database based on an identifier string
  void set(const String& entry_name, const RealSymMatrix& rsm);
  /// set a RealVectorArray within the database based on an identifier string
  void set(const String& entry_name, const RealVectorArray& rva);
  /// set an IntVectorArray within the database based on an identifier string
  void set(const String& entry_name, const IntVectorArray& iva);
  /// set an IntSetArray within the database based on an identifier string
  void set(const String& entry_name, const IntSetArray& isa);
  /// set a RealSetArray within the database based on an identifier string
  void set(const String& entry_name, const RealSetArray& rsa);
  /// set an IntRealMapArray within the database based on an identifier string
  void set(const String& entry_name, const IntRealMapArray& irma);
  /// set a RealRealMapArray within the database based on an identifier string
  void set(const String& entry_name, const RealRealMapArray& rrma);
  /// set a StringArray within the database based on an identifier string
  void set(const String& entry_name, const StringArray& sa);

  /// function to check dbRep (does this envelope contain a letter)
  bool is_null() const;

protected:

  //
  //- Heading: Constructors
  //

  /// constructor initializes the base class part of letter classes
  /// (BaseConstructor overloading avoids infinite recursion in the
  /// derived class constructors - Coplien, p. 139)
  ProblemDescDB(BaseConstructor, ParallelLibrary& parallel_lib);

  //
  //- Heading: Virtual functions
  //

  /// derived class specifics within parse_inputs()
  virtual void derived_parse_inputs(const char* dakota_input_file,
				    const char* parser_options);
  /// derived class specifics within broadcast()
  virtual void derived_broadcast();
  /// derived class specifics within post_process()
  virtual void derived_post_process();

  //
  //- Heading: Data
  //
 
  // The data objects that comprise the problem specification resulting
  // either from kwhandler (parser) or insert_node (library mode) calls.

  /// the strategy specification (only one allowed) resulting from a call
  /// to strategy_kwhandler() or insert_node()
  DataStrategy strategySpec;
  /// list of method specifications, one for each call to method_kwhandler()
  /// or insert_node()
  std::list<DataMethod> dataMethodList;
  /// list of model specifications, one for each call to model_kwhandler()
  /// or insert_node()
  std::list<DataModel> dataModelList;
  /// list of variables specifications, one for each call to
  /// variables_kwhandler() or insert_node()
  std::list<DataVariables> dataVariablesList;
  /// list of interface specifications, one for each call to
  /// interface_kwhandler() or insert_node()
  std::list<DataInterface> dataInterfaceList;
  /// list of responses specifications, one for each call to
  /// responses_kwhandler() or insert_node()
  std::list<DataResponses> dataResponsesList;

  /// counter for strategy specifications used in check_input
  size_t strategyCntr;

private:

  //
  //- Heading: Private convenience functions
  //

  // These functions avoid multiple instantiations of the same specification.

  /// retrieve an existing Iterator, if it exists, or instantiate a new one
  const Iterator& get_iterator(Model& model);
  /// retrieve an existing Model, if it exists, or instantiate a new one
  const Model& get_model();
  /// retrieve an existing Variables, if it exists, or instantiate a new one
  const Variables& get_variables();
  /// retrieve an existing Interface, if it exists, or instantiate a new one
  const Interface& get_interface();
  /// retrieve an existing Response, if it exists, or instantiate a new one
  const Response& get_response(const Variables& vars);

  /// Used by the envelope constructor to instantiate the correct letter class
  ProblemDescDB* get_db(ParallelLibrary& parallel_lib);

  /// MPI send of a large buffer containing strategySpec and all objects in
  /// dataMethodList, dataModelList, dataVariablesList, dataInterfaceList,
  /// and dataResponsesList.  Used by manage_inputs().
  void send_db_buffer();
  /// MPI receive of a large buffer containing strategySpec and all objects
  /// in dataMethodList, dataModelList, dataVariablesList, dataInterfaceList,
  /// and dataResponsesList.  Used by manage_inputs().
  void receive_db_buffer();

  /// echo the (potentially) specified input file to stdout
  void echo_input_file(const char* dakota_input_file);

  //
  //- Heading: Data
  //
 
  /// reference to the parallel_lib object passed from main
  ParallelLibrary& parallelLib;

  // Iterators for identifying active list nodes in data object linked lists

  /// iterator identifying the active list node in dataMethodList
  std::list<DataMethod>::iterator dataMethodIter;
  /// iterator identifying the active list node in dataModelList
  std::list<DataModel>::iterator dataModelIter;
  /// iterator identifying the active list node in dataVariablesList
  std::list<DataVariables>::iterator dataVariablesIter;
  /// iterator identifying the active list node in dataInterfaceList
  std::list<DataInterface>::iterator dataInterfaceIter;
  /// iterator identifying the active list node in dataResponsesList
  std::list<DataResponses>::iterator dataResponsesIter;

  /// list of iterator objects, one for each method specification
  IteratorList iteratorList;
  /// list of model objects, one for each model specification
  ModelList modelList;
  /// list of variables objects, one for each variables specification
  VariablesList variablesList;
  /// list of interface objects, one for each interface specification
  InterfaceList interfaceList;
  /// list of response objects, one for each responses specification
  ResponseList responseList;

  /// prevents use of get_<type> retrieval and set_<type> update functions 
  /// prior to setting the list node for the active method specification
  bool methodDBLocked;
  /// prevents use of get_<type> retrieval and set_<type> update functions 
  /// prior to setting the list node for the active model specification
  bool modelDBLocked;
  /// prevents use of get_<type> retrieval and set_<type> update functions 
  /// prior to setting the list node for the active variables specification
  bool variablesDBLocked;
  /// prevents use of get_<type> retrieval and set_<type> update functions 
  /// prior to setting the list node for the active interface specification
  bool interfaceDBLocked;
  /// prevents use of get_<type> retrieval and set_<type> update functions 
  /// prior to setting the list node for the active responses specification
  bool responsesDBLocked;

  /// pointer to the letter (initialized only for the envelope)
  ProblemDescDB* dbRep;
  /// number of objects sharing dbRep
  int referenceCount;

};


inline void ProblemDescDB::lock()
{
  if (dbRep)
    dbRep->methodDBLocked = dbRep->modelDBLocked = dbRep->variablesDBLocked
      = dbRep->interfaceDBLocked = dbRep->responsesDBLocked = true;
  else
    methodDBLocked = modelDBLocked = variablesDBLocked = interfaceDBLocked
      = responsesDBLocked = true;
}


inline void ProblemDescDB::unlock()
{
  if (dbRep)
    dbRep->methodDBLocked = dbRep->modelDBLocked = dbRep->variablesDBLocked
      = dbRep->interfaceDBLocked = dbRep->responsesDBLocked = false;
  else
    methodDBLocked = modelDBLocked = variablesDBLocked = interfaceDBLocked
      = responsesDBLocked = false;
}


inline ParallelLibrary& ProblemDescDB::parallel_library() const
{ return (dbRep) ? dbRep->parallelLib : parallelLib; }


inline IteratorList& ProblemDescDB::iterator_list()
{ return (dbRep) ? dbRep->iteratorList : iteratorList; }


inline ModelList& ProblemDescDB::model_list()
{ return (dbRep) ? dbRep->modelList : modelList; }


inline VariablesList& ProblemDescDB::variables_list()
{ return (dbRep) ? dbRep->variablesList : variablesList; }


inline InterfaceList& ProblemDescDB::interface_list()
{ return (dbRep) ? dbRep->interfaceList : interfaceList; }


inline ResponseList& ProblemDescDB::response_list()
{ return (dbRep) ? dbRep->responseList : responseList; }


inline size_t ProblemDescDB::get_db_method_node()
{
  if (dbRep)
    return dbRep->get_db_method_node();
  else {
    return std::distance(dataMethodList.begin(), dataMethodIter);
  }
}


inline size_t ProblemDescDB::get_db_model_node()
{
  if (dbRep)
    return dbRep->get_db_model_node();
  else {
    return std::distance(dataModelList.begin(), dataModelIter);
  }
}


inline void ProblemDescDB::insert_node(const DataStrategy& data_strategy)
{
  if (dbRep) {
    dbRep->strategySpec = data_strategy;
    dbRep->strategyCntr++;
  }
  else {
    strategySpec = data_strategy;
    strategyCntr++;
  }
}


inline void ProblemDescDB::insert_node(const DataMethod& data_method)
{
  if (dbRep)
    dbRep->dataMethodList.push_back(data_method);
  else
    dataMethodList.push_back(data_method);
}


inline void ProblemDescDB::insert_node(const DataModel& data_model)
{
  if (dbRep)
    dbRep->dataModelList.push_back(data_model);
  else
    dataModelList.push_back(data_model);
}


inline void ProblemDescDB::insert_node(DataVariables& data_variables)
{
  if (dbRep)
    dbRep->dataVariablesList.push_back(data_variables);
  else
    dataVariablesList.push_back(data_variables);
}


inline void ProblemDescDB::insert_node(const DataInterface& data_interface)
{
  if (dbRep)
    dbRep->dataInterfaceList.push_back(data_interface);
  else
    dataInterfaceList.push_back(data_interface);
}


inline void ProblemDescDB::insert_node(const DataResponses& data_responses)
{
  if (dbRep)
    dbRep->dataResponsesList.push_back(data_responses);
  else
    dataResponsesList.push_back(data_responses);
}


inline bool ProblemDescDB::is_null() const
{ return (dbRep) ? false : true; }


} // namespace Dakota

#endif
