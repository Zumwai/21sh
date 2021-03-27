/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:01:21 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 14:13:38 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_PARSER_H
# define INPUT_PARSER_H



int		parse_incoming_subline(t_norm *norm, int size);
int		verify_char_heredoc(char c);


#endif
