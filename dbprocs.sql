create or replace function get_by_exemption(exemption_val integer)
returns table(id integer, name text, address text, exemption integer) language plpgsql as $func$
BEGIN
    return query 
	select employers.id, employers.name, employers.address, employers.exemption
    from public.employers where public.employers.exemption = exemption_val;
END;	
$func$;

CREATE OR REPLACE PROCEDURE delete_employer (exemption_val integer) LANGUAGE plpgsql AS 
$$
   BEGIN 
      DELETE FROM public.employers WHERE public.employers.exemption = exemption_val;
   END;
$$;

CREATE OR REPLACE PROCEDURE update_employer(
    name   TEXT,
    address  TEXT,
    exemption integer,
    id  integer) LANGUAGE plpgsql  AS $$
BEGIN
    UPDATE public.employer
       SET  name = update_employer.name,
            address = update_employer.address,
            exemption = update_employer.exemption
     WHERE employer.id = update_employer.id;
END;
$$;

CREATE OR REPLACE PROCEDURE public.create_employer(
	IN arg_name text,
	IN arg_address text,
	IN arg_exemption integer)
LANGUAGE 'plpgsql'
AS $BODY$
BEGIN
    INSERT INTO public.employers (name, address, exemption) 
	VALUES (arg_name, arg_address, arg_exemption);
END;
$BODY$;