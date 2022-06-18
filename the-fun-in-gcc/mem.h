#include <stddef.h>

typedef union header {
	struct {
		union header *next;
		unsigned size; // including this header
	} h;
	long align; // for alignment only
} Header;

static Header base;
static Header *freep = NULL;

void freem(void *ap)
{
	Header *bp, *p;
	bp = (Header *) ap - 1; // point to header

	// find corresponding slot for bp (between p and p->h.next)
	for (p = freep; !(bp > p && bp < p->h.next); p = p->h.next)
		if (p >= p->h.next && (bp < p->h.next || bp > p))
			break; // bp is the first or last block
	if (bp + bp->h.size == p->h.next) { // absorb next block
		bp->h.size += p->h.next->h.size;
		bp->h.next = p->h.next->h.next;
	} else
		bp->h.next = p->h.next;
	if (p + p->h.size == bp) { // merge into previous block
		p->h.size += bp->h.size;
		p->h.next = bp->h.next;
	} else
		p->h.next = bp;
	freep = p;
}

#define NALLOC 1024

static Header *morecore(unsigned nunits)
{
	char *cp, *sbrk(int);
	Header *up;

	if (nunits < NALLOC)
		nunits = NALLOC;
	cp = sbrk(nunits * sizeof(Header));
	if (cp == (char *) -1)
		return NULL;
	up = (Header *) cp;
	up->h.size = nunits;
	freem((void *) (up+1));
	return freep;
}

void *allocm(unsigned nbytes)
{
	Header *p, *prevp;
	unsigned nunits = 1 + (nbytes+sizeof(Header)-1)/sizeof(Header);

	if ((prevp = freep) == NULL) {
		base.h.next = freep = prevp = &base;
		base.h.size = 0;
	}

	for (p = prevp->h.next; ; prevp = p, p = p->h.next) {
		if (p->h.size >= nunits) {
			if (p->h.size == nunits)
				prevp->h.next = p->h.next;
			else {
				p->h.size -= nunits;
				p += p->h.size;
				// p now points to new Header
				p->h.size = nunits;
			}
			freep = prevp;
			return (void *) (p+1);
		}
		if (p == freep)
			if ((p = morecore(nunits)) == NULL)
				return NULL;
	}
}

#include <stdio.h>
void memstate()
{
	Header *p = &base;
	if (!freep) {
		printf("allocm owns no memory yet\n");
		return;
	}

	do {
		printf("[%d]", p->h.size * sizeof(Header));
		if (p == freep)
			printf("(*)     ");
		else
			printf("        ");
	} while ((p = p->h.next) != &base);
	putchar('\n');
}
