




(setf requirements(cdadr degree))


(defun get-required-degree (degree))                   //gets the degree requirements
    (cdr (car (cdr person))))



(defun get-transcript (person))                   ;gets just the transcript
    (cdr (car person)))


(defun get-plan (person))
     (cdadr person))                                ;gets just the plan 

(defun get-total-hours (person catalog)
	(+ (get-transcript-hours (get-transcript person) catalog)
	   (get-plan-hours (get-plan person) catalog)))



(defun get-class-hours (class catalog)
     (cond ((null catalog) 
 	      0)
	   ((eql class (caar catalog))
	     (cadar catalog))
	   (t (get-class-hours class (cdr catalog))) ))        ;this is the default case



(defun get-plan-hours (plan catalog)             ;get hours only for the plan
   (cond ((null plan)
	0)
	(t
		(+ 
		   (get-class-hours (car plan) catalog)
		   (get-plan-hours (cdr plan) catalog) ))))
   

(setf *passing-grades* '(A A- B+ B B- C+ C C-))




(defun get-transcript-hours (transcript catalog)
  (cond ((null transcript)
	 0)
	((member (cadar transcript) *passing-grades*)
	 (+ (get-class-hours (caar transcript) catalog)
	    (get-transcript-hours (cdr transcript) catalog)))
	(t
	  (get-transcript-hours (cdr transcript) catalog ))))



(defun get-passed-classes (person)
	(if (null (get-transcript (person))) nil)
	(if (member (car (get-transcript(person))) *passing-grades*)
		(cons (car (get-transcript(person))) (get-passed-classes (  WHAT DO I PUT HERE???  )




(defun get-missing-classes (person degree)
	(if (null requirements) nil) 
	(if (and (member (car requirements) (or (get-transcript (person)) (get-plan (person))))  (member (get-transcript (person)    ))
	    (get-missing-classes (person (cdr requirements))) 
	(cons (car requirements) (get-missing-classes (person (cdr requirements)))))
