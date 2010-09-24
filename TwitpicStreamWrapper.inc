<?php
// $Id: $

/**
 * Twitpic Stream Wrapper
 *
 * This class provides a complete stream wrapper implementation.
 *
 * @see http://www.php.net/manual/en/class.streamwrapper.php 
 */
class TwitpicStreamWrapper implements DrupalStreamWrapperInterface {
  /**
   * Stream context resource.
   *
   * @var Resource
   */
  public $context;

  /**
   * A generic resource handle.
   *
   * @var Resource
   */
  public $handle = NULL;

  /**
   * Instance URI (stream).
   *
   * A stream is referenced as "scheme://target".
   *
   * @var String
   */
  protected $uri;

  /**
   * Base implementation of setUri().
   */
  function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Base implementation of getUri().
   */
  function getUri() {
    return $this->uri;
  }

  /**
   * Base implementation of getMimeType().
   */
  static function getMimeType($uri, $mapping = NULL) {
    return 'application/jpeg';
  }

  /**
   * Base implementation of chmod().
   */
  function chmod($mode) {
    return FALSE;
  }

  /**
   * Base implementation of realpath().
   */
  function realpath() {
    return FALSE;
  }

  /**
   * Support for fopen(), file_get_contents(), file_put_contents() etc.
   *
   * @param $uri
   *   A string containing the URI to the file to open.
   * @param $mode
   *   The file mode ("r", "wb" etc.).
   * @param $options
   *   A bit mask of STREAM_USE_PATH and STREAM_REPORT_ERRORS.
   * @param &$opened_path
   *   A string containing the path actually opened.
   * @return
   *   Returns TRUE if file was opened successfully.
   * @see http://php.net/manual/en/streamwrapper.stream-open.php
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    $allowed_modes = array('r', 'rb');
    if (!in_array($mode, $allowed_modes)) {
      return FALSE;
    }

    $this->uri = $uri;
    $url = $this->getExternalUrl();
    
    if ($options & STREAM_REPORT_ERRORS) {
      $this->handle = fopen($url, $mode);
    }
    else {
      $this->handle = @fopen($url, $mode);      
    }

    return (bool)$this->handle;
  }

  /**
   * Support for flock().
   *
   * @param $operation
   *   One of the following:
   *   - LOCK_SH to acquire a shared lock (reader).
   *   - LOCK_EX to acquire an exclusive lock (writer).
   *   - LOCK_UN to release a lock (shared or exclusive).
   *   - LOCK_NB if you don't want flock() to block while locking (not
   *     supported on Windows).
   * @return
   *   Always returns TRUE at the present time.
   * @see http://php.net/manual/en/streamwrapper.stream-lock.php
   */
  public function stream_lock($operation) {
    return FALSE;
  }

  /**
   * Support for fread(), file_get_contents() etc.
   *
   * @param $count
   *   Maximum number of bytes to be read.
   * @return
   *   The string that was read, or FALSE in case of an error.
   * @see http://php.net/manual/en/streamwrapper.stream-read.php
   */
  public function stream_read($count) {
    return fread($this->handle, $count);
  }

  /**
   * Support for fwrite(), file_put_contents() etc.
   *
   * @param $data
   *   The string to be written.
   * @return
   *   The number of bytes written (integer).
   * @see http://php.net/manual/en/streamwrapper.stream-write.php
   */
  public function stream_write($data) {
    return FALSE;
  }

  /**
   * Support for feof().
   *
   * @return
   *   TRUE if end-of-file has been reached.
   * @see http://php.net/manual/en/streamwrapper.stream-eof.php
   */
  public function stream_eof() {
    return feof($this->handle);
  }

  /**
   * Support for fseek().
   *
   * @param $offset
   *   The byte offset to got to.
   * @param $whence
   *   SEEK_SET, SEEK_CUR, or SEEK_END.
   * @return
   *   TRUE on success.
   * @see http://php.net/manual/en/streamwrapper.stream-seek.php
   */
  public function stream_seek($offset, $whence) {
    return fseek($this->handle, $offset, $whence);
  }

  /**
   * Support for fflush().
   *
   * @return
   *   TRUE if data was successfully stored (or there was no data to store).
   * @see http://php.net/manual/en/streamwrapper.stream-flush.php
   */
  public function stream_flush() {
    return fflush($this->handle);
  }

  /**
   * Support for ftell().
   *
   * @return
   *   The current offset in bytes from the beginning of file.
   * @see http://php.net/manual/en/streamwrapper.stream-tell.php
   */
  public function stream_tell() {
    return ftell($this->handle);
  }

  /**
   * Support for fstat().
   *
   * @return
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   * @see http://php.net/manual/en/streamwrapper.stream-stat.php
   */
  public function stream_stat() {
    return FALSE;
  }

  /**
   * Support for fclose().
   *
   * @return
   *   TRUE if stream was successfully closed.
   * @see http://php.net/manual/en/streamwrapper.stream-close.php
   */
  public function stream_close() {
    return fclose($this->handle);
  }

  /**
   * Support for unlink().
   *
   * @param $uri
   *   A string containing the uri to the resource to delete.
   * @return
   *   TRUE if resource was successfully deleted.
   * @see http://php.net/manual/en/streamwrapper.unlink.php
   */
  public function unlink($uri) {
    return FALSE;
  }

  /**
   * Support for rename().
   *
   * @param $from_uri,
   *   The uri to the file to rename.
   * @param $to_uri
   *   The new uri for file.
   * @return
   *   TRUE if file was successfully renamed.
   * @see http://php.net/manual/en/streamwrapper.rename.php
   */
  public function rename($from_uri, $to_uri) {
    return FALSE;
  }

  /**
   * Support for mkdir().
   *
   * @param $uri
   *   A string containing the URI to the directory to create.
   * @param $mode
   *   Permission flags - see mkdir().
   * @param $options
   *   A bit mask of STREAM_REPORT_ERRORS and STREAM_MKDIR_RECURSIVE.
   * @return
   *   TRUE if directory was successfully created.
   * @see http://php.net/manual/en/streamwrapper.mkdir.php
   */
  public function mkdir($uri, $mode, $options) {
    return FALSE;
  }

  /**
   * Support for rmdir().
   *
   * @param $uri
   *   A string containing the URI to the directory to delete.
   * @param $options
   *   A bit mask of STREAM_REPORT_ERRORS.
   * @return
   *   TRUE if directory was successfully removed.
   * @see http://php.net/manual/en/streamwrapper.rmdir.php
   */
  public function rmdir($uri, $options) {
    return FALSE;
  }

  /**
   * Support for stat().
   *
   * @param $uri
   *   A string containing the URI to get information about.
   * @param $flags
   *   A bit mask of STREAM_URL_STAT_LINK and STREAM_URL_STAT_QUIET.
   * @return
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   * @see http://php.net/manual/en/streamwrapper.url-stat.php
   */
  public function url_stat($uri, $flags) {
    $this->uri = $uri;
    if ($flags & STREAM_URL_STAT_QUIET) {
      return @stat($this->getLocalPath());
    }
    else {
      return stat($this->getLocalPath());
    }
  }

  /**
   * Support for opendir().
   *
   * @param $uri
   *   A string containing the URI to the directory to open.
   * @param $options
   *   Unknown (parameter is not documented in PHP Manual).
   * @return
   *   TRUE on success.
   * @see http://php.net/manual/en/streamwrapper.dir-opendir.php
   */
  public function dir_opendir($uri, $options) {
    $this->uri = $uri;
    $this->handle = opendir($this->getLocalPath());

    return (bool)$this->handle;
  }

  /**
   * Support for readdir().
   *
   * @return
   *   The next filename, or FALSE if there are no more files in the directory.
   * @see http://php.net/manual/en/streamwrapper.dir-readdir.php
   */
  public function dir_readdir() {
    return FALSE;
  }

  /**
   * Support for rewinddir().
   *
   * @return
   *   TRUE on success.
   * @see http://php.net/manual/en/streamwrapper.dir-rewinddir.php
   */
  public function dir_rewinddir() {
    return FALSE;
  }

  /**
   * Support for closedir().
   *
   * @return
   *   TRUE on success.
   * @see http://php.net/manual/en/streamwrapper.dir-closedir.php
   */
  public function dir_closedir() {
    return FALSE;
  }

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    return '';
  }

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URL of a Twitpic image.
   */
  function getExternalUrl() {
    // Get image_id and size from the URI into a structured array.
    $target = file_uri_target($this->uri);
    $options = array_combine(
      array('image_id', 'size'),
      explode('/', $target)
    );
    
    // If no size is specified, default to thumb.
    if (empty($options['size'])) {
      $options['size'] = 'thumb';
    }
    
    // Create the URL
    $url = 'http://twitpic.com/show/' . $options['size'] . '/' . $options['image_id'];
    return $url;    
  }

  /**
   * Gets the name of the directory from a given path.
   *
   * This method is usually accessed through drupal_dirname(), which wraps
   * around the PHP dirname() function because it does not support stream
   * wrappers.
   *
   * @param $uri
   *   A URI or path.
   *
   * @return
   *   A string containing the directory name.
   *
   * @see drupal_dirname()
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $target  = $this->getTarget($uri);
    $dirname = dirname($target);

    if ($dirname == '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }
}
