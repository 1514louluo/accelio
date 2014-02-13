/*
 * Copyright (c) 2013 Mellanox Technologies®. All rights reserved.
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file COPYING in the main
 * directory of this source tree, or the Mellanox Technologies® BSD license
 * below:
 *
 *      - Redistribution and use in source and binary forms, with or without
 *        modification, are permitted provided that the following conditions
 *        are met:
 *
 *      - Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *      - Neither the name of the Mellanox Technologies® nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "xio_os.h"
#include "libxio.h"
#include "xio_tls.h"


#if 0
/*---------------------------------------------------------------------------*/
/* struct	                                                             */
/*---------------------------------------------------------------------------*/
struct xio_thread_data {
	int  _xio_errno;
};

/*---------------------------------------------------------------------------*/
/* globals	                                                             */
/*---------------------------------------------------------------------------*/
static pthread_key_t thread_data_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static inline struct xio_thread_data *xio_thread_data_get(void);

/*---------------------------------------------------------------------------*/
/* xio_thread_data_init	                                                     */
/*---------------------------------------------------------------------------*/
static struct xio_thread_data *xio_thread_data_init(void)
{
	int	retval;
	struct xio_thread_data *td = ucalloc(1, sizeof(*td));
	if (td == NULL) {
		ERROR_LOG("FATAL ERROR: can't create TLS value\n");
	}

	/* it not ok for NULL */
	retval = pthread_setspecific(thread_data_key, td);
	if (retval)
		ERROR_LOG("FATAL ERROR: pthread_setspecific failed. %d %p %d\n",
			  retval, td, thread_data_key);

	return td;
}

/*---------------------------------------------------------------------------*/
/* xio_thread_data_free	                                                     */
/*---------------------------------------------------------------------------*/
static void xio_thread_data_free(void *ptr)
{
	    ufree(ptr);
	    pthread_key_delete(thread_data_key);
}

/*---------------------------------------------------------------------------*/
/* xio_thread_data_construct						     */
/*---------------------------------------------------------------------------*/
void xio_thread_data_construct(void)
{
      int retval;
      retval = pthread_key_create(&thread_data_key, xio_thread_data_free);
      if (retval)
		ERROR_LOG("FATAL ERROR: pthread_key_create failed. %d\n",
			  retval);

}

/*---------------------------------------------------------------------------*/
/* xio_thread_data_destruct						     */
/*---------------------------------------------------------------------------*/
void xio_thread_data_destruct(void)
{
	/* for main thread only */
	ufree(xio_thread_data_get());
	pthread_key_delete(thread_data_key);
}

/*---------------------------------------------------------------------------*/
/* xio_thread_data_get	                                                     */
/*---------------------------------------------------------------------------*/
static inline struct xio_thread_data *xio_thread_data_get(void)
{

	struct xio_thread_data	*td;
	pthread_once(&key_once, xio_thread_data_construct);

	td = pthread_getspecific(thread_data_key);
	if (td  == NULL) {
		xio_thread_data_init();
		td = pthread_getspecific(thread_data_key);
		if (td == NULL)
			ERROR_LOG("FATAL ERROR: can't create TLS\n");
	}
	return td;
}

/*---------------------------------------------------------------------------*/
/* debugging facilities							     */
/*---------------------------------------------------------------------------*/
void xio_set_error(int errnum) { xio_thread_data_get()->_xio_errno = errnum; }

/*---------------------------------------------------------------------------*/
/* xio_errno								     */
/*---------------------------------------------------------------------------*/
int xio_errno(void) { return xio_thread_data_get()->_xio_errno; }

#endif


/*---------------------------------------------------------------------------*/
/* global tls	                                                             */
/*---------------------------------------------------------------------------*/
static __thread int _xio_errno;


/*---------------------------------------------------------------------------*/
/* xio_thread_data_destruct						     */
/*---------------------------------------------------------------------------*/
void xio_thread_data_destruct(void)
{
}

/*---------------------------------------------------------------------------*/
/* xio_thread_data_construct						     */
/*---------------------------------------------------------------------------*/
void xio_thread_data_construct(void)
{
}

/*---------------------------------------------------------------------------*/
/* debugging facilities							     */
/*---------------------------------------------------------------------------*/
void xio_set_error(int errnum) { _xio_errno = errnum; }


/*---------------------------------------------------------------------------*/
/* xio_errno								     */
/*---------------------------------------------------------------------------*/
int xio_errno(void) { return _xio_errno; }
