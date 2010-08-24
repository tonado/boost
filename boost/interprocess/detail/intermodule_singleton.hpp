//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2009-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_INTERMODULE_SINGLETON_HPP
#define BOOST_INTERPROCESS_INTERMODULE_SINGLETON_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/detail/tmp_dir_helpers.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/detail/mpl.hpp>
#include <boost/assert.hpp>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#if (defined BOOST_INTERPROCESS_WINDOWS)
#include <fcntl.h>
#include <io.h>

#include <sys/locking.h>
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace boost{
namespace interprocess{
namespace detail{

namespace file_locking_helpers {

inline void get_pid_creation_time_str(std::string &s)
{
   std::stringstream stream;
   stream << get_current_process_id() << '_';
   stream.precision(6);
   stream << std::fixed << get_current_process_creation_time();
   s = stream.str();
}

inline void create_tmp_subdir_and_get_pid_based_filepath(const char *subdir_name, const char *file_prefix, OS_process_id_t pid, std::string &s, bool creation_time = false)
{
   //Let's create a lock file for each process gmem that will mark if
   //the process is alive or not
   create_tmp_and_clean_old(s);
   s += "/";
   s += subdir_name;
   if(!open_or_create_directory(s.c_str())){
      throw interprocess_exception(error_info(system_error_code()));
   }
   s += "/";
   s += file_prefix;
   if(creation_time){
      std::string sstamp;   
      get_pid_creation_time_str(sstamp);
      s += sstamp;
   }
   else{
      pid_str_t pid_str;
      get_pid_str(pid_str, pid);
      s += pid_str;
   }
}

inline bool check_if_filename_complies_with_pid
   (const char *filename, const char *prefix, OS_process_id_t pid, std::string &file_suffix, bool creation_time = false)
{
   //Check if filename complies with lock file name pattern
   std::string fname(filename);
   std::string fprefix(prefix);
   if(fname.size() <= fprefix.size()){
      return false;
   }
   fname.resize(fprefix.size());
   if(fname != fprefix){
      return false;
   }

   //If not our lock file, delete it if we can lock it
   fname = filename;
   fname.erase(0, fprefix.size());
   pid_str_t pid_str;
   get_pid_str(pid_str, pid);
   file_suffix = pid_str;
   if(creation_time){
      std::size_t p = fname.find('_');
      if (p == std::string::npos){
         return false;
      }
      std::string save_suffix(fname);
      fname.erase(p);
      fname.swap(file_suffix);
      bool ret = (file_suffix == fname);
      file_suffix.swap(save_suffix);
      return ret;
   }
   else{
      fname.swap(file_suffix);
      return (file_suffix == fname);
   }
}

}  //file_locking_helpers

namespace intermodule_singleton_helpers {

const int GMemMarkToBeRemoved = -1;
const int GMemNotPresent      = -2;

inline const char *get_lock_file_subdir_name()
{  return "gmem";  }

inline const char *get_lock_file_base_name()
{  return "lck";  }

inline void create_and_get_singleton_lock_file_path(std::string &s)
{
   file_locking_helpers::create_tmp_subdir_and_get_pid_based_filepath
      (get_lock_file_subdir_name(), get_lock_file_base_name(), get_current_process_id(), s, true);
}

inline const char *get_shm_base_name()
{  return "bip.gmem.shm.";  }

inline void get_shm_name(std::string &shm_name)
{
   file_locking_helpers::get_pid_creation_time_str(shm_name);
   shm_name.insert(0, get_shm_base_name());
}

inline std::size_t get_shm_size()
{  return 65536;  }

inline void apply_gmem_erase_logic(const char *filepath, const char *filename);

inline bool remove_old_gmem()
{
   std::string refcstrRootDirectory;
   tmp_folder(refcstrRootDirectory);
   refcstrRootDirectory += "/";
   refcstrRootDirectory += get_lock_file_subdir_name();
   return for_each_file_in_dir(refcstrRootDirectory.c_str(), apply_gmem_erase_logic);
}

#if (defined BOOST_INTERPROCESS_WINDOWS)
/*
inline bool remove_old_gmem()
{
   void *             hFile;                       // Handle to directory
   winapi::win32_find_data_t  FileInformation;     // File information

   //Get base directory
   std::string str;
   tmp_folder(str);
   str += "/";
   str += get_lock_file_subdir_name();
   const std::size_t base_root_dir_len = str.size();

   //Find all files and directories
   str  +=  "\\*.*";
   hFile = winapi::find_first_file(str.c_str(), &FileInformation);
   if(hFile != winapi::invalid_handle_value){
      do{   //Now loop every file
         str.erase(base_root_dir_len);
         //If it's not "." or ".." skip it
         if(FileInformation.cFileName[0] != '.'){
            str += "\\";   str += FileInformation.cFileName;
            //If it's a file, apply erase logic
            if(!(FileInformation.dwFileAttributes & winapi::file_attribute_directory)){
               apply_gmem_erase_logic(str.c_str(), FileInformation.cFileName);
            }
         }
      //Go to the next file
      } while(winapi::find_next_file(hFile, &FileInformation) == 1);

      // Close handle and see if the loop has ended with an error
      winapi::find_close(hFile);
      if(winapi::get_last_error() != winapi::error_no_more_files){
         return false;
      }
   }
   return true;
}
*/

struct locking_file_serial_id
{
   int fd;
   unsigned long dwVolumeSerialNumber;
   unsigned long nFileIndexHigh;
   unsigned long nFileIndexLow;
   //This reference count counts the number of modules attached
   //to the shared memory and lock file. This serves to unlink
   //the locking file and shared memory when all modules are
   //done with the global memory (shared memory)
   volatile boost::uint32_t modules_attached_to_gmem_count;
};

inline bool lock_locking_file(int fd)
{
   int ret = 0;
   while(ret != 0 && errno == EDEADLK){
      ret = _locking(fd, _LK_LOCK, 1/*lock_file_contents_length()*/);
   }
   return 0 == ret;
}

inline bool try_lock_locking_file(int fd)
{
   return 0 == _locking(fd, _LK_NBLCK , 1);
}

inline int open_or_create_and_lock_file(const char *name)
{
   permissions p;
   p.set_unrestricted();
   while(1){
      file_handle_t handle = create_or_open_file(name, read_write, p);
      int fd = _open_osfhandle((intptr_t)handle, _O_TEXT);
      if(fd < 0){
         close_file(handle);
         return fd;
      }
      if(!try_lock_locking_file(fd)){
         _close(fd);
         return -1;
      }
      struct _stat s;
      if(0 == _stat(name, &s)){
         return fd;
      }
      else{
         _close(fd);
      }
   }
}

inline int try_open_and_lock_file(const char *name)
{
   file_handle_t handle = open_existing_file(name, read_write);
   int fd = _open_osfhandle((intptr_t)handle, _O_TEXT);
   if(fd < 0){
      close_file(handle);
      return fd;
   }
   if(!try_lock_locking_file(fd)){
      _close(fd);
      return -1;
   }
   return fd;
}

inline void close_lock_file(int fd)
{  _close(fd); }

inline bool is_valid_fd(int fd)
{
   struct _stat s;
   return EBADF != _fstat(fd, &s);
}

inline bool is_normal_file(int fd)
{
   if(_isatty(fd))
      return false;
   struct _stat s;
   if(0 != _fstat(fd, &s))
      return false;
   return 0 != (s.st_mode & _S_IFREG);
}

inline std::size_t get_size(int fd)
{
   struct _stat s;
   if(0 != _fstat(fd, &s))
      return 0u;
   return (std::size_t)s.st_size;
}

inline bool fill_file_serial_id(int fd, locking_file_serial_id &id)
{
   winapi::interprocess_by_handle_file_information info;
   if(!winapi::get_file_information_by_handle((void*)_get_osfhandle(fd), &info))
      return false;
   id.fd = fd;
   id.dwVolumeSerialNumber = info.dwVolumeSerialNumber;
   id.nFileIndexHigh = info.nFileIndexHigh;
   id.nFileIndexLow = info.nFileIndexLow;
   id.modules_attached_to_gmem_count = 1; //Initialize attached count
   return true;
}

inline bool compare_file_serial(int fd, const locking_file_serial_id &id)
{
   winapi::interprocess_by_handle_file_information info;
   if(!winapi::get_file_information_by_handle((void*)_get_osfhandle(fd), &info))
      return false;

   return   id.dwVolumeSerialNumber == info.dwVolumeSerialNumber  &&
            id.nFileIndexHigh       == info.nFileIndexHigh        &&
            id.nFileIndexLow        == info.nFileIndexLow;
}

#else //UNIX
/*
inline bool remove_old_gmem()
{
   std::string refcstrRootDirectory;
   tmp_folder(refcstrRootDirectory);
   refcstrRootDirectory += "/";
   refcstrRootDirectory += get_lock_file_subdir_name();

   DIR *d = opendir(refcstrRootDirectory.c_str());
   if(!d) {
      return false;
   }

   struct dir_close
   {
      DIR *d_;
      dir_close(DIR *d) : d_(d) {}
      ~dir_close() { ::closedir(d_); }
   } dc(d); (void)dc;

   struct ::dirent *de;
   struct ::stat st;
   std::string fn;

   while((de=::readdir(d))) {
      if( de->d_name[0] == '.' && ( de->d_name[1] == '\0'
            || (de->d_name[1] == '.' && de->d_name[2] == '\0' )) ){
         continue;
      }
      fn = refcstrRootDirectory;
      fn += '/';
      fn += de->d_name;

      //if(std::remove(fn.c_str())) {
         if(::stat(fn.c_str(), & st)) {
            return false;
         }
         //If it's a file, apply erase logic
         if(!S_ISDIR(st.st_mode)) {
            apply_gmem_erase_logic(fn.c_str(), de->d_name);
         }
      //}
   }
   return true;
}
*/
struct locking_file_serial_id
{
   int fd;
   dev_t st_dev;
   ino_t st_ino;
   //This reference count counts the number of modules attached
   //to the shared memory and lock file. This serves to unlink
   //the locking file and shared memory when all modules are
   //done with the global memory (shared memory)
   volatile boost::uint32_t modules_attached_to_gmem_count;
};

inline bool lock_locking_file(int fd)
{
   int ret = 0;
   while(ret != 0 && errno != EINTR){
      struct flock lock;
      lock.l_type = F_WRLCK;
      lock.l_whence = SEEK_SET;
      lock.l_start = 0;
      lock.l_len = 1;
      ret = fcntl (fd, F_SETLKW, &lock);
   }
   return 0 == ret;
}

inline bool try_lock_locking_file(int fd)
{
   struct flock lock;
   lock.l_type = F_WRLCK;
   lock.l_whence = SEEK_SET;
   lock.l_start = 0;
   lock.l_len = 1;
   return 0 == fcntl (fd, F_SETLK, &lock);
}

inline int open_or_create_and_lock_file(const char *name)
{
   permissions p;
   p.set_unrestricted();
   while(1){
      int fd = create_or_open_file(name, read_write, p);
      if(fd < 0){
         return fd;
      }
      if(!try_lock_locking_file(fd)){
         close(fd);
         return -1;
      }
      struct stat s;
      if(0 == stat(name, &s)){
         return fd;
      }
      else{
         close(fd);
      }
   }
}

inline int try_open_and_lock_file(const char *name)
{
   int fd = open_existing_file(name, read_write);
   if(fd < 0){
      return fd;
   }
   if(!try_lock_locking_file(fd)){
      close(fd);
      return -1;
   }
   return fd;
}

inline void close_lock_file(int fd)
{  close(fd); }

inline bool is_valid_fd(int fd)
{
   struct stat s;
   return EBADF != fstat(fd, &s);
}

inline bool is_normal_file(int fd)
{
   struct stat s;
   if(0 != fstat(fd, &s))
      return false;
   return 0 != (s.st_mode & S_IFREG);
}

inline std::size_t get_size(int fd)
{
   struct stat s;
   if(0 != fstat(fd, &s))
      return 0u;
   return (std::size_t)s.st_size;
}

inline bool fill_file_serial_id(int fd, locking_file_serial_id &id)
{
   struct stat s;
   if(0 != fstat(fd, &s))
      return false;
   id.fd = fd;
   id.st_dev = s.st_dev;
   id.st_ino = s.st_ino;
   id.modules_attached_to_gmem_count = 1; //Initialize attached count
   return true;
}

inline bool compare_file_serial(int fd, const locking_file_serial_id &id)
{
   struct stat info;
   if(0 != fstat(fd, &info))
      return false;

   return   id.st_dev == info.st_dev  &&
            id.st_ino == info.st_ino;
}

#endif

struct gmem_erase_func
{
   gmem_erase_func(const char *shm_name, const char *singleton_lock_file_path, managed_shared_memory & shm)
      :shm_name_(shm_name), singleton_lock_file_path_(singleton_lock_file_path), shm_(shm)
   {}

   void operator()()
   {
      locking_file_serial_id *pserial_id = shm_.find<locking_file_serial_id>("lock_file_fd").first;
      if(pserial_id){
         pserial_id->fd = GMemMarkToBeRemoved;
      }
      delete_file(singleton_lock_file_path_);
      shared_memory_object::remove(shm_name_);
   }
   
   const char * const shm_name_;
   const char * const singleton_lock_file_path_;
   managed_shared_memory & shm_;
};

//This function applies shared memory erasure logic based on the passed lock file.
inline void apply_gmem_erase_logic(const char *filepath, const char *filename)
{
   int fd = GMemMarkToBeRemoved;
   try{
      std::string str;
      //If the filename is current process lock file, then avoid it
      if(file_locking_helpers::check_if_filename_complies_with_pid
         (filename, get_lock_file_base_name(), get_current_process_id(), str, true)){
         return;
      }
      //Open and lock the other process' lock file
      fd = try_open_and_lock_file(filepath);
      if(fd < 0){
         return;
      }
      //If done, then the process is dead so take global shared memory name
      //(the name is based on the lock file name) and try to apply erasure logic
      str.insert(0, get_shm_base_name());
      try{
         managed_shared_memory shm(open_only, str.c_str());
         gmem_erase_func func(str.c_str(), filepath, shm);
         shm.try_atomic_func(func);
      }
      catch(interprocess_exception &e){
         //If shared memory is not found erase the lock file
         if(e.get_error_code() == not_found_error){
            delete_file(filepath);
         }
      }
   }
   catch(...){

   }
   if(fd >= 0){
      close_lock_file(fd);
   }
}

}  //namespace intermodule_singleton_helpers {



namespace intermodule_singleton_helpers {

//The lock file logic creates uses a unique instance to a file
struct lock_file_logic
{
   lock_file_logic(managed_shared_memory &shm)
      : mshm(shm)
   {}

   locking_file_serial_id * register_lock_file(int fd)
   {
      locking_file_serial_id *pinfo = mshm.construct<locking_file_serial_id>("lock_file_fd")();
      fill_file_serial_id(fd, *pinfo);
      return pinfo;
   }

   void operator()(void)
   {
      retry_with_new_shm = false;

      //First find the file locking descriptor id
      locking_file_serial_id *pserial_id =
         mshm.find<locking_file_serial_id>("lock_file_fd").first;

      int fd;
      //If not found schedule a creation
      if(!pserial_id){
         fd = GMemNotPresent;
      }
      //Else get it
      else{
         fd = pserial_id->fd;
      }
      //If we need to create a new one, do it
      if(fd == GMemNotPresent){
         std::string lck_str;
         //Create a unique current pid based lock file path
         create_and_get_singleton_lock_file_path(lck_str);
         //Open or create and lock file
         int fd = intermodule_singleton_helpers::open_or_create_and_lock_file(lck_str.c_str());
         //If failed, write a bad file descriptor to notify other modules that
         //something was wrong and unlink shared memory. Mark the function object
         //to tell caller to retry with another shared memory
         if(fd < 0){
            this->register_lock_file(GMemMarkToBeRemoved);
            std::string s;
            get_shm_name(s);
            shared_memory_object::remove(s.c_str());
            retry_with_new_shm = true;
         }
         //If successful, register the file descriptor
         else{
            this->register_lock_file(fd);
         }
      }
      //If the fd was invalid (maybe a previous try failed) notify caller that
      //should retry creation logic, since this shm might have been already
      //unlinked since the shm was removed
      else if (fd == GMemMarkToBeRemoved){
         retry_with_new_shm = true;
      }
      //If the stored fd is not valid (a open fd, a normal file with the
      //expected size, or does not have the same file id number,
      //then it's an old shm from an old process with the same pid.
      //If that's the case, mark it as invalid
      else if(!is_valid_fd(fd) ||
            !is_normal_file(fd) ||
            0 != get_size(fd) ||
            !compare_file_serial(fd, *pserial_id)){
         pserial_id->fd = GMemMarkToBeRemoved;
         std::string s;
         get_shm_name(s);
         shared_memory_object::remove(s.c_str());
         retry_with_new_shm = true;
      }
      else{
         //If the lock file is ok, increment reference count of
         //attached modules to shared memory
         atomic_inc32(&pserial_id->modules_attached_to_gmem_count);
      }
   }

   managed_shared_memory &mshm;
   bool retry_with_new_shm;
};

}  //namespace intermodule_singleton_helpers {

//This class contains common code for all singleton types, so that we instantiate this
//code just once per module. This class also holds a reference counted shared memory
//to be used by all instances

template<int Dummy>
class intermodule_singleton_common
{
   public:
   typedef void*(singleton_constructor_t)(managed_shared_memory &);
   typedef void (singleton_destructor_t)(void *, managed_shared_memory &);

   static const ::boost::uint32_t Uninitialized       = 0u;
   static const ::boost::uint32_t Initializing        = 1u;
   static const ::boost::uint32_t Initialized         = 2u;
   static const ::boost::uint32_t Broken              = 3u;

   static void finalize_singleton_logic(void *ptr, singleton_destructor_t destructor)
   {
      if(ptr)
         destructor(ptr, get_shm());
      //If this is the last singleton of this module
      //apply shm destruction.
      //Note: singletons are destroyed when the module is unloaded
      //so no threads should be executing or holding references
      //to this module
      if(1 == atomic_dec32(&this_module_singleton_count)){
         destroy_shm();
      }
   }

   static void initialize_singleton_logic
      (void *&ptr, volatile boost::uint32_t &this_module_singleton_initialized, singleton_constructor_t ini_func);

   private:
   static managed_shared_memory &get_shm()
   {
      return *static_cast<managed_shared_memory *>(static_cast<void *>(&shm_mem));
   }

   static const std::size_t MemSize = ((sizeof(managed_shared_memory)-1)/sizeof(max_align))+1u;
   static void initialize_shm();
   static void destroy_shm();
   //Static data, zero-initalized without any dependencies
   //this_module_singleton_count is the number of singletons used by this module
   static volatile boost::uint32_t this_module_singleton_count;
   //this_module_shm_initialized is the state of this module's shm class object
   static volatile boost::uint32_t this_module_shm_initialized;
   static max_align shm_mem[MemSize];
};

template<int Dummy>
volatile boost::uint32_t intermodule_singleton_common<Dummy>::this_module_singleton_count;

//template<int Dummy>
//const std::size_t intermodule_singleton_common<Dummy>::MemSize;

template<int Dummy>
volatile boost::uint32_t intermodule_singleton_common<Dummy>::this_module_shm_initialized;

template<int Dummy>
max_align intermodule_singleton_common<Dummy>::shm_mem[intermodule_singleton_common<Dummy>::MemSize];

template<int Dummy>
void intermodule_singleton_common<Dummy>::initialize_shm()
{
   //Obtain unique shm name and size
   std::string s;
   intermodule_singleton_helpers::get_shm_name(s);
   const char *ShmName = s.c_str();
   const std::size_t ShmSize = intermodule_singleton_helpers::get_shm_size();;
   while(1){
      //Try to pass shm state to initializing
      ::boost::uint32_t tmp = atomic_cas32(&this_module_shm_initialized, Initializing, Uninitialized);
      if(tmp >= Initialized){
         break;
      }
      //If some other thread is doing the work wait
      else if(tmp == Initializing){
         thread_yield();
      }
      else{ //(tmp == Uninitialized)
         //If not initialized try it again?
         try{
            //Remove old shared memory from the system
            intermodule_singleton_helpers::remove_old_gmem();
            //in-place construction of the shared memory class
            ::new (&get_shm())managed_shared_memory(open_or_create, ShmName, ShmSize);
            //Use shared memory internal lock to initialize the lock file
            //that will mark this gmem as "in use".
            intermodule_singleton_helpers::lock_file_logic f(get_shm());
            get_shm().atomic_func(f);
            //If function failed (maybe a competing process has erased the shared
            //memory between creation and file locking), retry with a new instance.
            if(f.retry_with_new_shm){
               get_shm().~managed_shared_memory();
               atomic_write32(&this_module_shm_initialized, Uninitialized);
            }
            else{
               //Locking succeeded, so this shared memory module-instance is ready
               atomic_write32(&this_module_shm_initialized, Initialized);
               break;
            }
         }
         catch(...){
            //
            throw;
         }
      }
   }
}

struct unlink_shmlogic
{
   unlink_shmlogic(managed_shared_memory &mshm)
      : mshm_(mshm)
   {}
   void operator()()
   {
      intermodule_singleton_helpers::locking_file_serial_id *pserial_id =
         mshm_.find<intermodule_singleton_helpers::locking_file_serial_id>
            ("lock_file_fd").first;
      BOOST_ASSERT(0 != pserial_id);
      if(1 == atomic_dec32(&pserial_id->modules_attached_to_gmem_count)){
         int fd = pserial_id->fd;
         if(fd > 0){
            pserial_id->fd = intermodule_singleton_helpers::GMemMarkToBeRemoved;
            std::string s;
            intermodule_singleton_helpers::create_and_get_singleton_lock_file_path(s);
            delete_file(s.c_str());
            intermodule_singleton_helpers::close_lock_file(fd);
            intermodule_singleton_helpers::get_shm_name(s);
            shared_memory_object::remove(s.c_str());
         }
      }
   }
   managed_shared_memory &mshm_;
};

template<int Dummy>
void intermodule_singleton_common<Dummy>::destroy_shm()
{
   if(!atomic_read32(&this_module_singleton_count)){
      //This module is being unloaded, so destroy
      //the shared memory object of this module
      //and unlink the shared memory if it's the last
      unlink_shmlogic f(get_shm());
      get_shm().atomic_func(f);
      (get_shm()).~managed_shared_memory();
      atomic_write32(&this_module_shm_initialized, Uninitialized);
      //Do some cleanup for other processes old gmem instances
      intermodule_singleton_helpers::remove_old_gmem();
   }
}

//Initialize this_module_singleton_ptr, creates the shared memory if needed and also creates an unique
//opaque type in shared memory through a singleton_constructor_t function call,
//initializing the passed pointer to that unique instance.
//
//We have two concurrency types here. a)the shared memory/singleton creation must
//be safe between threads of this process but in different modules/dlls. b)
//the pointer to the singleton is per-module, so we have to protect this
//initization between threads of the same module.
//
//All static variables declared here are shared between inside a module
//so atomic operations will synchronize only threads of the same module.
template<int Dummy>
void intermodule_singleton_common<Dummy>::initialize_singleton_logic
   (void *&ptr, volatile boost::uint32_t &this_module_singleton_initialized, singleton_constructor_t constructor)
{
   //If current module is not initialized enter to lock free logic
   if(atomic_read32(&this_module_singleton_initialized) != Initialized){
      //Now a single thread of the module will succeed in this CAS.
      //trying to pass from Uninitialized to Initializing
      ::boost::uint32_t previous_module_singleton_initialized = atomic_cas32
         (&this_module_singleton_initialized, Initializing, Uninitialized);
      //If the thread succeeded the CAS (winner) it will compete with other 
      //winner threads from other modules to create the shared memory
      if(previous_module_singleton_initialized == Uninitialized){
         try{
            //Now initialize shm, this function solves concurrency issues
            //between threads of several modules
            initialize_shm();
            //Increment the module reference count that reflects how many
            //singletons this module holds, so that we can safely destroy
            //module shared memory object when no singleton is left
            atomic_inc32(&this_module_singleton_count);
            //Now try to create the singleton in shared memory.
            //This function solves concurrency issues
            //between threads of several modules
            void *tmp = constructor(get_shm());
            //Insert a barrier before assigning the pointer to
            //make sure this assignment comes after the initialization
            atomic_write32(&this_module_singleton_initialized, Initializing);
            //Assign the singleton address to the module-local pointer
            ptr = tmp;
            //Memory barrier inserted, all previous operations should complete
            //before this one. Now marked as initialized
            atomic_inc32(&this_module_singleton_initialized);
         }
         catch(...){
            //Mark singleton failed to initialize
            atomic_write32(&this_module_singleton_initialized, Broken);
            throw;
         }
      }
      //If previous state was initializing, this means that another winner thread is
      //trying to initialize the singleton. Just wait until completes its work.
      else if(previous_module_singleton_initialized == Initializing){
         while(1){
            previous_module_singleton_initialized = atomic_read32(&this_module_singleton_initialized);
            if(previous_module_singleton_initialized >= Initialized){
               //Already initialized, or exception thrown by initializer thread
               break;
            }
            else if(previous_module_singleton_initialized == Initializing){
               detail::thread_yield();
            }
            else{
               //This can't be happening!
               BOOST_ASSERT(0);
            }
         }
      }
      else if(previous_module_singleton_initialized == Initialized){
         //Nothing to do here, the singleton is ready
      }
      //If previous state was greater than initialized, then memory is broken
      //trying to initialize the singleton.
      else{//(previous_module_singleton_initialized > Initialized)
         throw interprocess_exception("boost::interprocess::intermodule_singleton initialization failed");
      }
   }
   BOOST_ASSERT(ptr != 0);
}

//Now this class is a singleton, initializing the singleton in
//the first get() function call if LazyInit is false. If true
//then the singleton will be initialized when loading the module.
template<typename C, bool LazyInit = false>
class intermodule_singleton
{
   public:
   static C& get()   //Let's make inlining easy
   {
      if(!this_module_singleton_ptr){
         if(lifetime.dummy_function())  //This forces lifetime instantiation, for reference counted destruction
            intermodule_singleton_common<0>::initialize_singleton_logic
               (this_module_singleton_ptr, this_module_singleton_initialized, singleton_constructor);
      }
      return *static_cast<C*>(this_module_singleton_ptr);
   }

   struct ref_count_ptr
   {
      ref_count_ptr(C *p, boost::uint32_t count)
         : ptr(p), singleton_ref_count(count)
      {}
      C *ptr;
      //This reference count serves to count the number of attached
      //modules to this singleton
      volatile boost::uint32_t singleton_ref_count;
   };

   private:
   //These statics will be zero-initialized without any constructor call dependency
   //this_module_singleton_ptr will be a module-local pointer to the singleton
   static void*                      this_module_singleton_ptr;
   //this_module_singleton_count will be used to synchronize threads of the same module
   //for access to a singleton instance, and to flag the state of the
   //singleton.
   static volatile boost::uint32_t   this_module_singleton_initialized;

   //This class destructor will trigger singleton destruction
   struct lifetime_type_lazy
   {
      bool dummy_function()
      {  return m_dummy == 0; }

      ~lifetime_type_lazy()
      {
         intermodule_singleton_common<0>::finalize_singleton_logic
            (this_module_singleton_ptr, singleton_destructor);
      }
      //Dummy volatile so that the compiler can't resolve its value at compile-time
      //and can't avoid lifetime_type instantiation if dummy_function() is called.
      static volatile int m_dummy;
   };

   struct lifetime_type_static
      : public lifetime_type_lazy
   {
      lifetime_type_static()
      {
         intermodule_singleton_common<0>::initialize_singleton_logic
            (this_module_singleton_ptr, this_module_singleton_initialized, singleton_constructor);
      }
   };

   typedef typename if_c
      <LazyInit, lifetime_type_lazy, lifetime_type_static>::type lifetime_type;

   static lifetime_type lifetime;

   //A functor to be executed inside shared memory lock that just
   //searches for the singleton in shm and if not present creates a new one.
   //If singleton constructor throws, the exception is propagated
   struct init_atomic_func
   {
      init_atomic_func(managed_shared_memory &m)
         : mshm(m)
      {}

      void operator()()
      {
         ref_count_ptr *rcount = mshm.find<ref_count_ptr>(unique_instance).first;
         if(!rcount){
            C *p = new C();
            try{
               rcount = mshm.construct<ref_count_ptr>(unique_instance)(p, 0u);
            }
            catch(...){
               delete p;
               throw;
            }
         }
         atomic_inc32(&rcount->singleton_ref_count);
         ret_ptr = rcount->ptr;
      }
      managed_shared_memory &mshm;
      void *ret_ptr;
   };

   //A functor to be executed inside shared memory lock that just
   //deletes the singleton in shm if the attached count reaches to zero
   struct fini_atomic_func
   {
      fini_atomic_func(managed_shared_memory &m)
         : mshm(m)
      {}

      void operator()()
      {
         ref_count_ptr *rcount = mshm.find<ref_count_ptr>(unique_instance).first;
            //The object must exist
         BOOST_ASSERT(rcount);
         //Check if last reference
         if(atomic_dec32(&rcount->singleton_ref_count) == 1){
            //If last, destroy the object
            BOOST_ASSERT(rcount->ptr != 0);
            delete rcount->ptr;
            //Now destroy shm entry
            bool destroyed = mshm.destroy<ref_count_ptr>(unique_instance);
            (void)destroyed;  BOOST_ASSERT(destroyed == true);
         }
      }
      managed_shared_memory &mshm;
      void *ret_ptr;
   };

   //A wrapper to execute init_atomic_func
   static void *singleton_constructor(managed_shared_memory &mshm)
   {
      init_atomic_func f(mshm);
      mshm.atomic_func(f);
      return f.ret_ptr;
   }

   //A wrapper to execute fini_atomic_func
   static void singleton_destructor(void *p, managed_shared_memory &mshm)
   {  (void)p;
      fini_atomic_func f(mshm);
      mshm.atomic_func(f);
   }
};

template <typename C, bool L>
volatile int intermodule_singleton<C, L>::lifetime_type_lazy::m_dummy;

//These will be zero-initialized by the loader
template <typename C, bool L>
void *intermodule_singleton<C, L>::this_module_singleton_ptr;

template <typename C, bool L>
volatile boost::uint32_t intermodule_singleton<C, L>::this_module_singleton_initialized;

template <typename C, bool L>
typename intermodule_singleton<C, L>::lifetime_type intermodule_singleton<C, L>::lifetime;
  

}  //namespace detail{
}  //namespace interprocess{
}  //namespace boost{

#include <boost/interprocess/detail/config_end.hpp>

#endif


/*






                       if try_lock_file
                          open_shm_1
open_or_create_shm_1         
                          atomic_1
                            mark_to_be_removed
                            remove_file
                            remove_shm
                            close
                                                
atomic_1     
   if not present fd
     if(!create_and_lock) //delete in course, try again later
       mark_to_be_removed
       remove_shm_1
       try_again
     register_fd
   else if to_be_removed
     try_again
   else if not_valid_fd
     mark_to_be_removed
     remove_shm_1
     try_again
   else if(valid_fd)
      continue;
















Hint: fcntl is a nightmare because:

- to check if a file is locked you must open an new descriptor
- if you close any descriptor to that file all locks on that file are gone
- so each time you want to check something is locked without disburbing
  anything, you must leak that descriptor.

Hints to have only one open descriptor:

- Mark the lock file with a magic string.
- Store the file descriptor in shared memory.
- Open or create the shared memory:
    - If created, no problem,
         create a file and lock it in a loop testing that the file exists
    - If opened, take the the stored fd:
        - Take a lock on shared memory, if time exceeds too much time, unlink it and start again
        - IF it's a regular file (IFREG, using fstat to check) AND 
          its size is the expected (fstat) AND
          contains the magic number.
              LOCK IT
          ELSE
             the fd is invalid so unlink the shared memory and try to create it again


REMOVE_OLD_GMEM()

lock semaphore

loop all files in /tmp/boost_interprocess/gmem
  check pattern(name) #PID#.lock
  if (!pattern)
    continue;
  pid = extract_pid(name)
  if(pid == self_pid)
     continue;
  open(name);
  if(::fnctl(fd, GET_LCK)
     BOOST_ASSERT(flock.pid != self_pid)
     continue;
  shm_remove(bip_gmem_#PID#)
  unlink(name);

unlock_semaphore



STATIC_SINGLETON_CONSTRUCTOR 

remove_old_gmem()

  managed_shared_memory shm;
  bool completed = false;
  while(!completed){
     exclusive_lock(semaphore);
     try{
       managed_shared_memory tmp(create_only, bip_gmem_#SELFPID#, SIZE);
       shm = ::boost::move(tmp);
       if(!::open(/tmp/boost_interprocess/gmem/#PID#.lock) ||
          !::fnctl(fd, LOCK)
         throw whatever;
       leave file locked;
     }
     catch(interprocess_exception &e){
       if(e.error_code() != already_exists_error)){
         throw;
       }
       try{
          managed_shared_memory tmp(open_only, bip_gmem_#SELFPID#)
          shm = ::boost::move(tmp);
       }
       catch(...){
          if(e.error_code() != not_found_error)){
            throw;
          }
       }
     }
     completed = true;
  }
  ref_counted_type *rcount = shm.find_or_create<ref_counted_type>(unique_instance)(int(0));
  atomic_inc(rcount->cnt);

STATIC_SINGLETON_DESTRUCTOR

remove_old_mem()

exclusive_lock(semaphore)
  ref_count_type *rcount = shm.find_or_create<ref_count_type>("ref_count")(int(0));
  if(!atomic_dec(rcount->cnt))
    shm.destroy<T>(unique_instance);

*/
