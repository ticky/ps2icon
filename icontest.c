/*
* icontest 0.2.0 - Test and example program for libIcon
*
* Copyright (c) 2002 Corner <ma@placid.tv>
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy  of  this   software  and  associated   documentation  files  (the
* "Software"),  to deal  in the Software  without  restriction, including
* without  limitation  the rights to  use, copy,  modify, merge, publish,
* distribute,  sublicense, and/or  sell  copies of the  Software, and  to
* permit persons to  whom the Software is furnished  to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR   IMPLIED,  INCLUDING   BUT  NOT  LIMITED  TO   THE  WARRANTIES   OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR  PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE  AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR  ANY
* CLAIM,  DAMAGES  OR OTHER  LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT  OR OTHERWISE,  ARISING FROM,  OUT OF  OR IN  CONNECTION  WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ps2icon.h"

int main(int argc, char **argv)
{
	int	lFile, lVertices, lShapes;
	struct stat	lFs;
	void *lIconData;
	IconPtr *lIcon;
	Vertex *lV;

	if (argc != 2)
	{
		printf("usage: %s <file>\n", *argv);
		return 1;
	}

	/*
	 * Open an .icn icon file for testing, use fstat() to get filesize and malloc()
	 * a buffer for it.  Read the whole file into the buffer and close the file.
	 */
	argv++;
	printf("Opening file: %s\n", *argv);
	lFile = open(*argv, O_RDONLY, NULL);
	if (lFile < 0)
	{
		perror("error");
		return 1;
	}

	fstat(lFile, &lFs);

	if ((0 == lFs.st_size))
	{
		printf("error: File has zero size\n");
		return 1;
	}

	lIconData = malloc(lFs.st_size);
	read(lFile, lIconData, lFs.st_size);
	close(lFile);

	/* Check magic number to see that it is a icn file
	 * we do this both little and big endian here
	 */
	if ((0x0100 != (*(int32_t *) lIconData)) && (0x010000 != (*(int32_t *) lIconData)))
	{
		printf("error: Not an .icn file\n");
		return 1;
	}

	/*
	 * Initialize our icon in a IconPtr struct so that we can access icon data
	 * more freely by using struct pointers.  After initializing we throw away
	 * the icon buffer we allocated earlier.
	 */
	lIcon = iconInit(lIconData);
	free(lIconData);

	/*
	 * Now we are free to use the icon data in any way we want
	 */
	printf("Shapes..: %d\n", lIcon->ip_shapes);
	printf("Vertices: %d\n", lIcon->ip_vertices);

	lV = lIcon->ip_vertex;
	for (lShapes = 0; lShapes < lIcon->ip_shapes; lShapes++)
	{
		printf("Shape %d data..\n", lShapes);
		for (lVertices = 1; lVertices <= lIcon->ip_vertices; lVertices++)
		{
			printf("Vertice %3d:	", lVertices);
			printf("%1.3f, %1.3f, %1.3f\n", lV->v_x, lV->v_y, lV->v_z);
			lV++;
		}
	}

	/*
	 * To release all memory allocated by the call to IconInit() we must call
	 * IconDestroy() with a pointer to our IconPtr struct.
	 */
	iconDestroy(lIcon);

	return 0;
}
