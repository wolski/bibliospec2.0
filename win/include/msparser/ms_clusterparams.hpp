/*
##############################################################################
# file: ms_clusterparams.hpp                                                 #
# 'msparser' toolkit                                                         #
# Encapsulates "mascot.dat"-file that describes most important parameters    #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_clusterparams.hpp       $ #
#     $Author: davidc $ #
#       $Date: 2007-01-16 13:16:11 $ #
#   $Revision: 1.6 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_CLUSTERPARAMS_HPP
#define MS_CLUSTERPARAMS_HPP

#ifdef _WIN32
#pragma warning(disable:4251)   // Don't want all classes to be exported
#pragma warning(disable:4786)   // Debug symbols too long
#   ifndef _MATRIX_USE_STATIC_LIB
#       ifdef MS_MASCOTRESFILE_EXPORTS
#           define MS_MASCOTRESFILE_API __declspec(dllexport)
#       else
#           define MS_MASCOTRESFILE_API __declspec(dllimport)
#       endif
#   else
#       define MS_MASCOTRESFILE_API
#   endif
#else
#   define MS_MASCOTRESFILE_API
#endif

// for the sake of #include <string>
#ifdef __ALPHA_UNIX__
#include <ctype.h>
#endif
#include <string>


namespace matrix_science {
    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Lists all supported operating systems
    /*!
     * Mascot runs on a number of different operating systems. The 
     * following are currently supported. 
     * To access from perl, use, for example: <P><tt>
     * $x = $msparser::_OS_WINDOWS_NT
     * </tt></P>
     */
    enum OPERATING_SYS
    {
        _OS_AIX               = 0,   //!< IBM AIX
        _OS_UNKNOWN           = 1,   //!< Unknown OS
        _OS_WINDOWS_NT        = 2,   //!< WindowsXXX
        _OS_IRIX              = 3,   //!< Irix
        _OS_ALPHA_TRUE64      = 4,   //!< Alpha Tru64
        _OS_SOLARIS           = 5,   //!< Solaris
        _OS_LINUX             = 6,   //!< Linux
        _OS_LINUXALPHA        = 7,   //!< Alpha Linux
        _OS_FREE_BSD_         = 8,   //!< FreeBSD
        _OS_NUM_OPERATING_SYS = 9    //!< Placeholder
    };

    //! An instance of this class represents all the parameters specified in <b>Cluster</b>-section of <tt>mascot.dat</tt>
    /*!
     *  An instance of this class is created and polulated in ms_datfile-class. 
     *  It can also be created separately and initialized with default values. 
     *  One can create an instance of the class or copy from another instance 
     *  in order to pass it then as an parameters-containing object.
     *  For detailed information on any of the options please consult Mascot manual.<BR>
     *  Also get yourselves acquainted with the base class ms_customproperty. 
     *  It facilitates the following tasks:
     *  <ul>
     *  <li>retrieving an unsupported property</li>
     *  <li>retrieving raw/text/xml property representation</li>
     *  <li>checking for existence of a certain property rather than 
     *  dealing with its default value</li>
     *  <li>accessing commented lines in a section</li>
     *  </ul>
     *  More functionality is described in details in the documentation for the class.
     */
    class MS_MASCOTRESFILE_API ms_clusterparams: public ms_customproperty
    {
        friend class ms_datfile;
    public:
        //! Default constructor
        ms_clusterparams();

        //! Copying constructor
        ms_clusterparams(const ms_clusterparams& src);

        //! Destructor
        virtual ~ms_clusterparams();

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_clusterparams* right);

        //! Assignment operator for C++ client applications
        ms_clusterparams& operator=(const ms_clusterparams& right);

        //! Tells whether the section has been actually read from the file
        /*!
         *  By default the <tt>Options</tt> section is unavailable until it has 
         *  been set to a different state.
         */
        bool isSectionAvailable() const;

        //! Changes availability of the section, i.e. should it be saved in a file
        void setSectionAvailable(const bool value);

        //! Returns <b>TRUE</b> if <b>Enabled</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  Value <b>1</b> is to enable cluster mode, whereas <b>0</b> is to enable single server mode.
         *  Default value is <b>0</b>.
         */
        bool isEnabled() const;

        //! Allows to change <b>Enabled</b> parameter value
        /*!
         *  See #isEnabled() for more information.
         */
        void setEnabled(const bool flag);

        //! Returns an instance of ms_computeraddress-class created for <b>MasterComputerName</b> parameter
        /*!
         *  <b>MasterComputerName</b> parameter contains a host name for 
         *  the master computer and, optionally, the IP address separated by a comma. 
         *  See documentation for ms_computeraddress-class for information about 
         *  its parameters and default values.
         */
        const ms_computeraddress* getMasterComputer() const;

        //! Allows to change <b>MasterComputerName</b> parameter value
        /*!
         *  See #getIam() for more information.
         */
        void setMasterComputer(const ms_computeraddress* value);

        //! Returns value of <b>DefaultNodeOS</b> parameter value as one of the enumeration members
        /*!
         *  If no OS is defined for a particular node, then this parameter value is assumed as OS.
         *  See documentation for OPERATING_SYS type for a list of possible values.
         *  Default value is <b>_OS_WINDOWS_NT</b> (2).
         */
        OPERATING_SYS getDefaultNodeOS() const;

        //! Allows to change <b>DefaultNodeOS</b> parameter value
        /*!
         *  See #getDefaultNodeOS() for more information.
         */
        void setDefaultNodeOS(const OPERATING_SYS value);

        //! Returns value of <b>DefaultNodeHomeDir</b> parameter
        /*!
         *  If no specific home directory is specified for a particular node in <tt>nodelist.txt</tt>, 
         *  then the value of <b>DefaultNodeHomeDir</b> parameter is used.
         *  To override this setting for a particular node, enter the directory 
         *  on the node line in <tt>nodelist.txt</tt>.
         *  Default value for this parameter is <tt>/MascotNode</tt>-folder.
         *  \sa #getDefaultPort()
         */
        std::string getDefaultNodeHomeDir() const;

        //! Allows to change <b>DefaultNodeHomeDir</b> parameter value
        /*!
         *  See #getDefaultNodeHomeDir() for more information.
         */
        void setDefaultNodeHomeDir(const char* str);

        //! Returns value of <b>DefaultPort</b> parameter
        /*!
         *  If no port number is specified for a node in <tt>nodelist.txt</tt>, 
         *  then this port number will be used.
         *  Default value for this parameter is <b>5001</b>.
         *  \sa #getDefaultNodeHomeDir()
         */
        int getDefaultPort() const;

        //! Allows to change <b>DefaultPort</b> parameter value
        /*!
         *  See #getDefaultPort() for more information.
         */
        void setDefaultPort(const int value);

        //! Returns value of <b>DefaultNodeHomeDirFromMaster</b> parameter value
        /*!
         *  This is the directory on the node as seen from the master. 
         *  For a Windows cluster, this must be present and specified as a UNC name.
	     *  For a Unix cluster, this parameter must be commented out.
         *  Default value is <tt>\\\\\<host_name\>\\c$\\mascotnode</tt>.
         */
        std::string getDefaultNodeHomeDirFromMaster() const;

        //! Allows to change <b>DefaultNodeHomeDirFromMaster</b> parameter value
        /*!
         *  See #getDefaultNodeHomeDirFromMaster() for more information.
         */
        void setDefaultNodeHomeDirFromMaster(const char* str);

        //! Returns value of <b>MascotNodeScript</b> parameter
        /*!
         *  <b>MascotNodeScript</b> parameter specifies a script name which 
         *  is run for each node with different parameters described
         *  in Mascot manual. By default it is empty.
         */
        std::string getMascotNodeScript() const;

        //! Allows to change <b>MascotNodeScript</b> parameter value
        /*!
         *  See #getMascotNodeScript() for more information.
         */
        void setMascotNodeScript(const char* str);

        //! Returns value of <b>MascotNodeRebootScript</b> parameter
        /*!
         *  <b>MascotNodeRebootScript</b> is the name of an optional script 
         *  to re-boot a cluster node. If this parameter is defined, 
         *  then there will be a link at the bottom of each Mascot Cluster Node 
         *  status page. When this link is clicked, <tt>ms-monitor</tt>
         *  will run the defined script on the master. 
         *  The host name of the specified node will be passed to the script 
         *  as a parameter.<BR>
         *  By default it is empty.
         */
        std::string getMascotNodeRebootScript() const;

        //! Allows to change <b>MascotNodeRebootScript</b> parameter value
        /*!
         *  See #getMascotNodeRebootScript() for more information.
         */
        void setMascotNodeRebootScript(const char* str);

        //! Returns number of <b>SubClusterSet</b> parameter entries which is a total number of sub-clusters
        /*!
         *  There might be several <b>SubClusterSet</b> parameter entries
         *  on separate lines in the configuration file. 
         *  Each parameter line has the following format:<BR>
         *  <tt>SubClusterSet X Y</tt><BR>
         *  Large clusters can be divided into sub-clusters. 
         *  <tt>X</tt> is a unique integer value (0-based) 
         *  used to identify the sub-cluster (from <b>0</b> to <b>49</b> inclusively).
         *  <tt>Y</tt> is the maximum number of processors in the sub-cluster. 
         *  A single cluster must have a single entry with <tt>X</tt> set to <b>0</b>.
         *  If no such entries are present in the file, one subcluster is assumed - default entry (<b>0</b>,<b>0</b>).
         *  Therefore, default value returned by this function is <b>1</b>.
         *  Use #getSubClusterID() and #getMaxCPUsPerSubCluster() members to retrieve the entries.
         */
        int getNumberOfSubClusters() const;

        //! Erases information about all sub clusters
        /*!
         *  One entry (<b>0</b>,<b>0</b>) for "this subcluster" stays in the list forever.
         */
        void clearSubClusters();

        //! Returns sub-cluster ID by its index
        /*!
         *  \param index a number between <b>0</b> and (#getNumberOfSubClusters()-1)
         *  \return an integer number which serves as a sub-cluster ID
         */
        int getSubClusterID(const int index) const;

        //! Returns <tt>Y</tt>-part of <b>SubClusterSet</b> entry given an index in the list
        /*!
         *  See #getNumberOfSubClusters() for more detailed explanations.
         *  \param index a number in the list (from <b>0</b> to (#getNumberOfSubClusters()-1))
         *  \return maximum number of CPUs for the specified sub-cluster (<b>0</b> by default)
         */
        int getSubClusterMaxCPU(const int index) const;

        //! Allows to add a new <b>SubClusterSet</b> entry
        /*!
         *  See #getNumberOfSubClusters() for more information.
         *  \param id unique ID identifying a sub-cluster (from <b>0</b> to <b>49</b>) to set number of CPUs for
         *  \param maxCPUs maximum number of CPUs for the specified sub-cluster (from <b>0</b> to <b>1024*64</b>)
         */
        void appendSubCluster(const int id, const int maxCPUs);

        // internal usage only
        int getThisSubClusterID() const;

        // internal usage only
        void setThisSubClusterID(const int id);

        //! Returns value of <b>IPCTimeout</b> parameter
        /*!
         *  <b>IPCTimeout</b> parameter is the timeout in seconds for 
         *  inter-process communication.
         *  Default value is <b>10</b> seconds.
         *  \sa #getIPCLogging(), #getIPCLogfile()
         */
        int getIPCTimeout() const;

        //! Allows to change <b>IPCTimeout</b> parameter value
        /*!
         *  See #getIPCTimeout() for more information.
         */
        void setIPCTimeout(const int value);

        //! Returns value of <b>IPCLogging</b> parameter
        /*!
         *  Parameter <b>IPCLogging</b> specifies logging level for 
         *  inter-process communications. It has the followings possible values:
         *  <ul>
         *  <li><b>0</b> for no logging of inter-process communication</li>
         *  <li><b>1</b> for for minimal logging</li>
         *  <li><b>2</b> for for verbose logging</li>
         *  </ul>
         *  Default value is <b>0</b>.
         *  \sa #getIPCTimeout()
         */
        int getIPCLogging() const;

        //! Allows to change <b>IPCLogging</b> parameter value
        /*!
         *  See #getIPCLogging() for more information.
         */
        void setIPCLogging(const int value);

        //! Returns value of <b>IPCLogfile</b> parameter
        /*!
         *  <b>IPCLogfile</b> parameter specifies a relative path 
         *  to the inter-process communication log file.
         *  Default value is <tt>../logs/IPC.log</tt>.
         *  \sa #getIPCTimeout()
         */
        std::string getIPCLogfile() const;

        //! Allows to change <b>IPCLogfile</b> parameter value
        /*!
         *  See #getIPCLogfile() for more information.
         */
        void setIPCLogfile(const char* str);

        //! Returns value of <b>CheckNodesAliveFreq</b> parameter
        /*!
         *  <b>getCheckNodesAliveFreq</b> parameter specifies 
         *  the interval in seconds between ‘health checks’ on the nodes.
         *  Default value is <b>30</b> sec.
         */
        int getCheckNodesAliveFreq() const;

        //! Allows to change <b>CheckNodesAliveFreq</b> parameter value
        /*!
         *  See #getCheckNodesAliveFreq() for more information.
         */
        void setCheckNodesAliveFreq(const int value);

        //! Returns value of <b>SecsToWaitForNodeAtStartup</b> parameter
        /*!
         *  <b>SecsToWaitForNodeAtStartup</b> parameter specifies a timeout.
         *  At startup, if a node is not available within this time, 
         *  the system will continue to startup without that node. 
         *  If the value is set to <b>0</b>, then the system will wait indefinitely.<BR>
         *  This timeout is also used if a node fails while the system is running. 
         *  The system will wait for this number of seconds before re-initialising 
         *  <tt>ms-monitor.exe</tt>. 
         *  This means that a short-lived interruption in network communication 
         *  doesn’t create a major service interruption.<BR>
         *  Default value is <b>60</b> sec.
         */
        int getSecsToWaitForNodeAtStartup() const;

        //! Allows to change <b>SecsToWaitForNodeAtStartup</b> parameter value
        /*!
         *  See #getSecsToWaitForNodeAtStartup() for more information.
         */
        void setSecsToWaitForNodeAtStartup(const int value);

    private:
        bool            sectionAvailable_;
        bool            enabled_;

        ms_computeraddress masterComputer_;

        OPERATING_SYS   defaultNodeOS_;
        std::string     szDefaultNodeHomeDir_;
        int             defaultPort_;
        std::string     szDefaultNodeHomeDirFromMaster_;
        std::string     szMascotNodeScript_;
        std::string     szMascotNodeRebootScript_;

        std::vector<int> subClusterIDs_;
        std::vector<int> maxCpusPerSubCluster_;
        bool             anySubCluster_;

        int             thisSubCluster_;

        int             IPCTimeout_;
        int             IPCLogging_;
        std::string     IPCLogfile_;
        int             checkNodesAliveFreq_;
        int             secsToWaitForNodeAtStartup_;
    }; // class ms_clusterparams
    /** @} */ // end of config_group
} // namespace matrix_science

#endif // MS_CLUSTERPARAMS_HPP

/*------------------------------- End of File -------------------------------*/
