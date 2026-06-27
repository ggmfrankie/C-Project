package sortAlgorithms;

import gui.Range;

/**
 * This is the implementation of MergeSort.
 * 
 * @author Franz-Josef
 * 
 */
public class MergeSort extends SortAlgorithm {

	private static final String[] SOURCE_CODE = new String[] {
	  "void merge(int links, int rechts) {",
	  "  int laenge = rechts - links + 1;",
	  "  int mitte = (links + rechts) / 2;",
	  "  int[] tempArray = new int[laenge];",
	  "  int l = links;",
	  "  int r = mitte + 1;",
	  "  int t = 0;",
	  "",
	  "  while(t < laenge) {",
	  "    if (r > rechts || (l <= mitte && elems[l] <= elems[r])) {",
	  "      tempArray[t] = elems[l];",
	  "      l = l + 1;",
	  "    } else {",
	  "      tempArray[t] = elems[r];",
	  "      r = r + 1;",
	  "    }",
	  "    t = t + 1;",
	  "  }",
	  "  System.arraycopy(tempArray, 0, elems, links, laenge);",
	  "}",
	  "",
	  "void mergesort(int links, int rechts) {",
	  "  if(links < rechts) {",
	  "    int mitte = (links + rechts) / 2;",
	  "    mergesort(links, mitte);",
	  "    mergesort(mitte + 1, rechts);",
	  "    merge(links, rechts);",
	  "  }",
	  "}"
	};

	@Override
	public String[] getSourceCode() {
		return SOURCE_CODE;
	}
	
	private void merge(int links, int rechts) {
		// remove mitte pointer set in mergesort: 
		aniHandler.removePointer("mitte");
		// für Übersichtlichkeit: links und rechts-pfeile entfernen
		aniHandler.removePointer("links");
		aniHandler.removePointer("rechts");
		setSourceCodeLine(1);
		int laenge = rechts - links + 1;
		setSourceCodeLine(2);
		int mitte = (links + rechts) / 2;
		aniHandler.setPointer("mitte", mitte);
		setSourceCodeLine(3);
		int[] tempArray = new int[laenge];
		aniHandler.createTempArray(links, laenge, "tempArray");
		setSourceCodeLine(4);
		int l = links;
		aniHandler.setPointer("l", l);
		setSourceCodeLine(5);
		int r = mitte + 1;
		aniHandler.setPointer("r", r);
		setSourceCodeLine(6);
		int t = 0;
		aniHandler.setTempPointer("t", t);
		setSourceCodeLine(8);
		
		while(t < laenge) {
			setSourceCodeLine(9);
			if (r > rechts || (l <= mitte && elems[l] <= elems[r])) {
				setSourceCodeLine(10);
				tempArray[t] = elems[l];
				aniHandler.loadItemIntoTempArray(l);
				setSourceCodeLine(11);
				l = l + 1;
				aniHandler.setPointer("l", l);
				setSourceCodeLine(16);
			} else {
				setSourceCodeLine(12);
				setSourceCodeLine(13);
				tempArray[t] = elems[r];
				aniHandler.loadItemIntoTempArray(r);
				setSourceCodeLine(14);
				r = r + 1;
				aniHandler.setPointer("r", r);
				setSourceCodeLine(16);
			}
			t = t + 1;
			aniHandler.setTempPointer("t", t);
			setSourceCodeLine(8);
		}
		setSourceCodeLine(18);
		System.arraycopy(tempArray, 0, elems, links, laenge);
		aniHandler.integrateTempArray();
		aniHandler.removePointer("l");
		aniHandler.removePointer("r");
		aniHandler.removePointer("t");
	}

	private void mergesort(int links, int rechts) {
		aniHandler.setPointer("links", links);
		aniHandler.setPointer("rechts", rechts);
		setSourceCodeLine(22);
		if(links < rechts) {
			setSourceCodeLine(23);
			int mitte = (links + rechts) / 2;
			aniHandler.setPointer("mitte", mitte);
			Range leftRange = aniHandler.addRange(links, mitte+1);
			setSourceCodeLine(24);		
			aniHandler.removePointer("mitte");
			mergesort(links, mitte);
			aniHandler.removeRange(leftRange);
			// restore pointers:
			aniHandler.setPointer("links", links);
			aniHandler.setPointer("mitte", mitte);
			aniHandler.setPointer("rechts", rechts);
			
			aniHandler.setPointer("mitte", mitte);
			Range rightRange = aniHandler.addRange(mitte+1, rechts+1);
			setSourceCodeLine(25);		
			aniHandler.removePointer("mitte");
			mergesort(mitte+1, rechts);
			aniHandler.removeRange(rightRange);
			// restore pointers:
			aniHandler.setPointer("links", links);
			aniHandler.setPointer("mitte", mitte);
			aniHandler.setPointer("rechts", rechts);
	
			setSourceCodeLine(26);		
			merge(links, rechts);
			// restore pointers:
			aniHandler.setPointer("links", links);
			aniHandler.setPointer("mitte", mitte);
			aniHandler.setPointer("rechts", rechts);
	    }
		setSourceCodeLine(27);
	}

	@Override
	public void run() {
		// elems = mergesort(elems, 0);
		mergesort(0, elems.length - 1);
		algorithmEnd();
	}

}
